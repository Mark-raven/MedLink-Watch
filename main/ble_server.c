#include "ble_server.h"
#include "app_uuid.h"
#include "host/util/util.h"

#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"

#include "medlink_service.h"

#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"

#include "host/ble_hs.h"

#include "esp_nimble_hci.h"

#include "host/ble_gap.h"
#include "host/ble_gatt.h"

#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

static const char *TAG = "BLE_SERVER";

static void medlink_advertise(void)
{
    struct ble_gap_adv_params adv_params;
    struct ble_hs_adv_fields fields;

    memset(&fields, 0, sizeof(fields));

    fields.flags = BLE_HS_ADV_F_DISC_GEN |
                   BLE_HS_ADV_F_BREDR_UNSUP;

    fields.tx_pwr_lvl_is_present = 1;
    fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;

    const char *name = ble_svc_gap_device_name();

    fields.name = (uint8_t *)name;
    fields.name_len = strlen(name);
    fields.name_is_complete = 1;

    int rc = ble_gap_adv_set_fields(&fields);

    if (rc != 0)
    {
        ESP_LOGE(TAG, "Advertising fields failed (%d)", rc);
        return;
    }

    memset(&adv_params, 0, sizeof(adv_params));

    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

    rc = ble_gap_adv_start(
            BLE_OWN_ADDR_PUBLIC,
            NULL,
            BLE_HS_FOREVER,
            &adv_params,
            NULL,
            NULL);

    if (rc != 0)
    {
        ESP_LOGE(TAG, "Advertising start failed (%d)", rc);
        return;
    }

    ESP_LOGI(TAG, "Advertising Started");

}

static void medlink_on_sync(void)
{
    ESP_LOGI(TAG, "BLE Host Synchronized");

    medlink_advertise();
}

void ble_host_task(void *param)
{
    ESP_LOGI(TAG, "BLE Host Started");

    nimble_port_run();

    nimble_port_freertos_deinit();
}

void ble_server_init(void)
{
    esp_err_t ret;

    ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());

        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "NVS Initialized");

    ret = nimble_port_init();

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize NimBLE (%d)", ret);
        return;
    }

    ble_hs_cfg.sync_cb = medlink_on_sync;

    ble_svc_gap_init();
    ble_svc_gatt_init();

    medlink_service_init();

    ble_svc_gap_device_name_set("MedLink_Watch");

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize NimBLE (%d)", ret);
        return;
    }

    ESP_LOGI(TAG, "NimBLE Initialized");

    nimble_port_freertos_init(ble_host_task);

}