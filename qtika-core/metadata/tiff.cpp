/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "stable.h"
#include "tiff.h"


namespace qtika {

namespace metadata {


TIFF::TIFF()
    : BITS_PER_SAMPLE(Property::internalIntegerSequence(QS("tiff:BitsPerSample")))
    , IMAGE_LENGTH(Property::internalInteger(QS("tiff:ImageLength")))
    , IMAGE_WIDTH(Property::internalInteger(QS("tiff:ImageWidth")))
    , SAMPLES_PER_PIXEL(Property::internalInteger(QS("tiff:SamplesPerPixel")))
    , FLASH_FIRED(Property::internalBoolean(QS("exif:Flash")))
    , EXPOSURE_TIME(Property::internalRational(QS("exif:ExposureTime")))
    , F_NUMBER(Property::internalRational(QS("exif:FNumber")))
    , FOCAL_LENGTH(Property::internalRational(QS("exif:FocalLength")))
    , ISO_SPEED_RATINGS(Property::internalIntegerSequence(QS("exif:IsoSpeedRatings")))
    , EQUIPMENT_MAKE(Property::internalText(QS("tiff:Make")))
    , EQUIPMENT_MODEL(Property::internalText(QS("tiff:Model")))
    , SOFTWARE(Property::internalText(QS("tiff:Software")))
    , ORIENTATION(Property::internalClosedChoice(QS("tiff:Orientation"), { "1", "2", "3", "4", "5", "6", "7", "8" }))
    , RESOLUTION_HORIZONTAL(Property::internalRational(QS("tiff:XResolution")))
    , RESOLUTION_VERTICAL(Property::internalRational(QS("tiff:YResolution")))
    , RESOLUTION_UNIT(Property::internalClosedChoice(QS("tiff:ResolutionUnit"), { "Inch", "cm" }))
    , ORIGINAL_DATE(Property::internalDate(QS("exif:DateTimeOriginal")))
{
}


}       // namespace metadata

}       // namespace qtika
