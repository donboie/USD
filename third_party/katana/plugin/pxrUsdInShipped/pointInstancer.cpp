//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
#include "pxrUsdInShipped/declareCoreOps.h"

#include "pxr/pxr.h"
#include "usdKatana/attrMap.h"
#include "usdKatana/readPointInstancer.h"
#include "usdKatana/utils.h"

#include "pxr/usd/usdGeom/pointInstancer.h"
#include "pxr/base/gf/matrix4d.h"

#include "pxrUsdInShipped/pointInstancerUtils.h"

PXR_NAMESPACE_USING_DIRECTIVE

PXRUSDKATANA_USDIN_PLUGIN_DEFINE(PxrUsdInCore_PointInstancerOp, privateData, interface)
{
    UsdGeomPointInstancer instancer =
        UsdGeomPointInstancer(privateData.GetUsdPrim());

    // Generate input attr map for consumption by the reader.
    //
    PxrUsdKatanaAttrMap inputAttrMap;

    // Get the instancer's Katana location.
    //
    inputAttrMap.set("outputLocationPath",
            FnKat::StringAttribute(interface.getOutputLocationPath()));

    //--------------------------------------------------------------------------
    // XXX At some point, instance matrix computation will get folded into
    // PxrUsdKatanaReadPointInstancer; until then, do the computation here and
    // add the result to the input attr map for the reader to use.
    //
    {
        const double currentTime = privateData.GetCurrentTime();

        // Gather frame-relative sample times and add them to the current time
        // to generate absolute sample times.
        //
        const std::vector<double> &motionSampleTimes =
            privateData.GetMotionSampleTimes(instancer.GetPositionsAttr());
        const size_t numSamples = motionSampleTimes.size();
        std::vector<UsdTimeCode> sampleTimes(numSamples);
        for (size_t a = 0; a < numSamples; ++a) {
            sampleTimes[a] = UsdTimeCode(currentTime + motionSampleTimes[a]);
        }

        // Compute the instancer's instance transforms.
        //
        std::vector<std::vector<GfMatrix4d>> xformSamples(numSamples);
        size_t numXformSamples = 0;
        PxrUsdInShipped_PointInstancerUtils::
            ComputeInstanceTransformsAtTime(
                xformSamples, numXformSamples, instancer, sampleTimes,
                UsdTimeCode(currentTime));
        if (numXformSamples == 0) {
            interface.setAttr("type", FnKat::StringAttribute("error"));
            interface.setAttr("errorMessage", FnKat::StringAttribute(
                                                  "Could not compute "
                                                  "sample/topology-invarying "
                                                  "instance transform matrix"));
            return;
        }

        size_t numInstances = xformSamples[0].size();

        // Add the result to the input attr map.
        //
        FnKat::DoubleBuilder instanceMatrixBldr(16);
        for (size_t a = 0; a < numXformSamples; ++a) {

            double relSampleTime = motionSampleTimes[a];

            // Shove samples into the builder at the frame-relative sample time.
            // If motion is backwards, make sure to reverse time samples.
            std::vector<double> &matVec = instanceMatrixBldr.get(
                privateData.IsMotionBackward()
                    ? PxrUsdKatanaUtils::ReverseTimeSample(relSampleTime)
                    : relSampleTime);

            matVec.reserve(16 * numInstances);
            for (size_t i = 0; i < numInstances; ++i) {

                GfMatrix4d instanceXform = xformSamples[a][i];
                const double *matArray = instanceXform.GetArray();

                for (int j = 0; j < 16; ++j) {
                    matVec.push_back(matArray[j]);
                }
            }
        }
        inputAttrMap.set("instanceMatrix", instanceMatrixBldr.build());
    }
    //--------------------------------------------------------------------------

    // Generate output attr maps.
    //
    // Instancer attr map: describes the instancer itself
    // Sources attr map: describes the instancer's "instance source" children.
    // Instances attr map: describes the instancer's "instance array" child.
    //
    PxrUsdKatanaAttrMap instancerAttrMap;
    PxrUsdKatanaAttrMap sourcesAttrMap;
    PxrUsdKatanaAttrMap instancesAttrMap;
    PxrUsdKatanaReadPointInstancer(
            instancer, privateData, instancerAttrMap, sourcesAttrMap,
            instancesAttrMap, inputAttrMap);

    // Send instancer attrs directly to the interface.
    //
    instancerAttrMap.toInterface(interface);

    // Early exit if any errors were encountered.
    //
    if (FnKat::StringAttribute(interface.getOutputAttr("type")
            ).getValue("", false) == "error")
    {
        return;
    }

    // Build the other output attr maps.
    //
    FnKat::GroupAttribute sourcesSSCAttrs = sourcesAttrMap.build();
    FnKat::GroupAttribute instancesSSCAttrs = instancesAttrMap.build();
    if (not sourcesSSCAttrs.isValid() or not instancesSSCAttrs.isValid())
    {
        return;
    }

    // Tell UsdIn to skip all children; we'll create them ourselves below.
    //
    interface.setAttr("__UsdIn.skipAllChildren", FnKat::IntAttribute(1));

    // Create "instance source" children using BuildIntermediate.
    //
    PxrUsdKatanaUsdInArgsRefPtr usdInArgs = privateData.GetUsdInArgs();
    FnKat::GroupAttribute childAttrs = sourcesSSCAttrs.getChildByName("c");
    for (int64_t i = 0; i < childAttrs.getNumberOfChildren(); ++i)
    {
        interface.createChild(
            childAttrs.getChildName(i),
            "PxrUsdIn.BuildIntermediate",
            FnKat::GroupBuilder()
                .update(interface.getOpArg())
                .set("staticScene", childAttrs.getChildByIndex(i))
                .build(),
            FnKat::GeolibCookInterface::ResetRootFalse,
            new PxrUsdKatanaUsdInPrivateData(
                    usdInArgs->GetRootPrim(), usdInArgs, &privateData),
            PxrUsdKatanaUsdInPrivateData::Delete);
    }

    // Create "instance array" child using StaticSceneCreate.
    //
    interface.execOp("StaticSceneCreate", instancesSSCAttrs);
}
