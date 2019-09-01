#include <fcntl.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <android-base/file.h>
#include <android-base/properties.h>
#include <android-base/strings.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "vendor_init.h"
#include "property_service.h"

char const *heapstartsize;
char const *heapgrowthlimit;
char const *heapsize;
char const *heapminfree;
char const *heapmaxfree;
char const *texture_cache_size;
char const *layer_cache_size;
char const *shape_cache_size;
char const *gradient_cache_size;
char const *drop_shadow_cache_size;

using android::base::GetProperty;
using android::base::ReadFileToString;
using android::init::property_set;

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void check_ram()
{
    struct sysinfo sys;

    sysinfo(&sys);

    if (sys.totalram > 3072ull * 1024 * 1024) {
        // original values in file framework/native: phone-xxhdpi-4096-dalvik-heap.mk
        // 4GB - from vince 7.1 values
        heapstartsize = "16m";
        heapgrowthlimit = "192m";
        heapsize = "512m";
        heapminfree = "4m";
        heapmaxfree = "8m";
        texture_cache_size="88"; //increased texture cachce size
        layer_cache_size="58"; //increased layer cachce size
        shape_cache_size="4";
        gradient_cache_size="1";
        drop_shadow_cache_size="6";
    } else if (sys.totalram > 2048ull * 1024 * 1024) {
        // original values in file framework/native: phone-xxhdpi-3072-dalvik-heap.mk
        // 3GB - from markw 6.0 values
        heapstartsize = "16m";
        heapgrowthlimit = "192m";
        heapsize = "512m";
        heapminfree = "8m";
        heapmaxfree = "32m";
        texture_cache_size="88"; //increased texture cachce size
        layer_cache_size="58"; //increased layer cachce size
        shape_cache_size="4";
        gradient_cache_size="1";
        drop_shadow_cache_size="6";
    } else {
        // from - phone-xxhdpi-2048-dalvik-heap.mk
        heapstartsize = "16m";
        heapgrowthlimit = "192m";
        heapsize = "512m";
        heapminfree = "2m";
        heapmaxfree = "8m";
        texture_cache_size="72";
        layer_cache_size="48";
        shape_cache_size="4";
        gradient_cache_size="1";
        drop_shadow_cache_size="6";
   }
}

void vendor_load_properties()
{
    check_ram();

    property_set("dalvik.vm.heapstartsize", heapstartsize);
    property_set("dalvik.vm.heapgrowthlimit", heapgrowthlimit);
    property_set("dalvik.vm.heapsize", heapsize);
    property_set("dalvik.vm.heaptargetutilization", "0.75");
    property_set("dalvik.vm.heapminfree", heapminfree);
    property_set("dalvik.vm.heapmaxfree", heapmaxfree);
}
