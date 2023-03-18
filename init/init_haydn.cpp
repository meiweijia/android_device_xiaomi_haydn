/*
   Copyright (C) 2020 The LineageOS Project.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <vector>

#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

using android::base::GetProperty;

std::vector<std::string> ro_props_default_source_order = {
    "",
    "bootimage.",
    "odm.",
    "product.",
    "system.",
    "system_ext.",
    "vendor.",
};

void property_override(char const prop[], char const value[], bool add = true)
{
    prop_info *pi;

    pi = (prop_info *)__system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else if (add)
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void set_ro_build_prop(const std::string &prop, const std::string &value)
{
    for (const auto &source : ro_props_default_source_order)
    {
        auto prop_name = "ro." + source + "build." + prop;
        if (source == "")
            property_override(prop_name.c_str(), value.c_str());
        else
            property_override(prop_name.c_str(), value.c_str(), false);
    }
};

void set_ro_product_prop(const std::string &prop, const std::string &value)
{
    for (const auto &source : ro_props_default_source_order)
    {
        auto prop_name = "ro.product." + source + prop;
        property_override(prop_name.c_str(), value.c_str(), false);
    }
};

void vendor_load_properties()
{
    std::string region;
    std::string sku;
    region = GetProperty("ro.boot.hwc", "");
    sku = GetProperty("ro.boot.hardware.sku", "");

    std::string model;
    std::string brand;
    std::string device = "haydn";
    std::string fingerprint;
    std::string description;
    std::string marketname;
    std::string mod_device;
    std::string product_sku = "nfc";

    bool is_india = (region == "IN");
    bool is_china = (region == "CN");

    brand = is_india ? "Xiaomi" : "Redmi";
    device = is_india ? "haydnin" : "haydn";
    model = is_india ? "M2012K11I" : (is_china ? "M2012K11C" : "M2012K11G");
    marketname = is_india ? "Mi 11X Pro" : (is_china ? (sku == "haydnpro" ? "Redmi K40 Pro+" : "Redmi K40 Pro") : "Mi 11i Global");
    mod_device = is_india ? "haydn_in_global" : (is_china ? "haydn" : "haydn_global");

    if (is_india)
    {
        fingerprint = "Xiaomi/haydn_in/haydnin:12/SKQ1.211001.001/V13.0.1.0.SKKINXM:user/release-keys";
        description = "haydnin-user 12 SKQ1.211001.001 V13.0.1.0.SKKINXM release-keys";
        product_sku = "";
    }
    else if (is_china)
    {
        fingerprint = "Redmi/" + sku + "/haydn:13/TKQ1.220829.002/V14.0.3.0.TKKCNXM:user/release-keys";
        description = sku + "-user 13 TKQ1.220829.002 V14.0.3.0.TKKCNXM release-keys";
    }
    else
    {
        fingerprint = "Redmi/haydn/haydn:12/SKQ1.211001.001/V13.0.1.0.SKKMIXM:user/release-keys";
        description = "haydn-user 12 SKQ1.211001.001 V13.0.1.0.SKKMIXM release-keys";
    }

    set_ro_build_prop("fingerprint", fingerprint);
    set_ro_product_prop("brand", brand);
    set_ro_product_prop("device", device);
    set_ro_product_prop("model", model);

    property_override("ro.build.description", description.c_str());
    property_override("ro.product.mod_device", mod_device.c_str());
    property_override("bluetooth.device.default_name", marketname.c_str());
    if (product_sku != "")
    {
        property_override("ro.boot.product.hardware.sku", product_sku.c_str());
    }
}
