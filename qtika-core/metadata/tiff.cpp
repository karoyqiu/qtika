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


Property TIFF::BITS_PER_SAMPLE()
{
    static const Property p = Property::internalIntegerSequence(QS("tiff:BitsPerSample"));
    return p;
}


Property TIFF::IMAGE_LENGTH()
{
    static const Property p = Property::internalInteger(QS("tiff:ImageLength"));
    return p;
}


Property TIFF::IMAGE_WIDTH()
{
    static const Property p = Property::internalInteger(QS("tiff:ImageWidth"));
    return p;
}


Property TIFF::SAMPLES_PER_PIXEL()
{
    static const Property p = Property::internalInteger(QS("tiff:SamplesPerPixel"));
    return p;
}


Property TIFF::FLASH_FIRED()
{
    static const Property p = Property::internalBoolean(QS("exif:Flash"));
    return p;
}


Property TIFF::EXPOSURE_TIME()
{
    static const Property p = Property::internalRational(QS("exif:ExposureTime"));
    return p;
}


Property TIFF::F_NUMBER()
{
    static const Property p = Property::internalRational(QS("exif:FNumber"));
    return p;
}


Property TIFF::FOCAL_LENGTH()
{
    static const Property p = Property::internalRational(QS("exif:FocalLength"));
    return p;
}


Property TIFF::ISO_SPEED_RATINGS()
{
    static const Property p = Property::internalIntegerSequence(QS("exif:IsoSpeedRatings"));
    return p;
}


Property TIFF::EQUIPMENT_MAKE()
{
    static const Property p = Property::internalText(QS("tiff:Make"));
    return p;
}


Property TIFF::EQUIPMENT_MODEL()
{
    static const Property p = Property::internalText(QS("tiff:Model"));
    return p;
}


Property TIFF::SOFTWARE()
{
    static const Property p = Property::internalText(QS("tiff:Software"));
    return p;
}


Property TIFF::ORIENTATION()
{
    static const Property p = Property::internalClosedChoice(QS("tiff:Orientation"), { "1", "2", "3", "4", "5", "6", "7", "8" });
    return p;
}


Property TIFF::RESOLUTION_HORIZONTAL()
{
    static const Property p = Property::internalRational(QS("tiff:XResolution"));
    return p;
}


Property TIFF::RESOLUTION_VERTICAL()
{
    static const Property p = Property::internalRational(QS("tiff:YResolution"));
    return p;
}


Property TIFF::RESOLUTION_UNIT()
{
    static const Property p = Property::internalClosedChoice(QS("tiff:ResolutionUnit"), { "Inch", "cm" });
    return p;
}


Property TIFF::ORIGINAL_DATE()
{
    static const Property p = Property::internalDate(QS("exif:DateTimeOriginal"));
    return p;
}


}       // namespace metadata

}       // namespace qtika
