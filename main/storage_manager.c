#include "storage_manager.h"

#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_err.h"

bool storage_manager_init(void)
{
    esp_err_t err = nvs_flash_init();

    if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
        err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    return err == ESP_OK;
}

bool storage_save_schedule(uint8_t hour,
                           uint8_t minute)
{
    nvs_handle_t nvsHandle;

    esp_err_t err = nvs_open("medlink", NVS_READWRITE, &nvsHandle);

    if (err != ESP_OK)
    {
        ESP_LOGE("STORAGE", "Failed to open NVS");

        return false;
    }

    err = nvs_set_u8(nvsHandle, "hour", hour);

    if (err == ESP_OK)
    {
        err = nvs_set_u8(nvsHandle, "minute", minute);
    }

    if (err == ESP_OK)
    {
        err = nvs_commit(nvsHandle);
    }

    nvs_close(nvsHandle);

    if (err == ESP_OK)
    {
        ESP_LOGI("STORAGE",
                 "Schedule Saved %02d:%02d",
                 hour,
                 minute);

        return true;
    }

    ESP_LOGE("STORAGE", "Failed to save schedule");

    return false;
}

bool storage_load_schedule(uint8_t *hour,
                           uint8_t *minute)
{
    nvs_handle_t nvsHandle;

    esp_err_t err = nvs_open("medlink", NVS_READONLY, &nvsHandle);

    if (err != ESP_OK)
    {
        ESP_LOGI("STORAGE", "No NVS namespace found");
        return false;
    }

    err = nvs_get_u8(nvsHandle, "hour", hour);

    if (err == ESP_OK)
    {
        err = nvs_get_u8(nvsHandle, "minute", minute);
    }

    nvs_close(nvsHandle);

    if (err == ESP_OK)
    {
        ESP_LOGI("STORAGE",
                 "Schedule Loaded %02d:%02d",
                 *hour,
                 *minute);

        return true;
    }

    ESP_LOGI("STORAGE", "No Saved Schedule");

    return false;
}