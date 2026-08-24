#include "medlink_service.h"
#include "medlink_protocol.h"

#include "esp_log.h"

#include "host/ble_gatt.h"
#include "host/ble_uuid.h"

#include "host/ble_hs_mbuf.h"

#include "host/ble_hs.h"

static const ble_uuid128_t medlink_service_uuid =
    BLE_UUID128_INIT(
        0xF0, 0xDE, 0xBC, 0x9A,
        0x78, 0x56,
        0x34, 0x12,
        0x78, 0x56,
        0x34, 0x12,
        0x78, 0x56,
        0x34, 0x12);

static const ble_uuid128_t medlink_rx_uuid =
    BLE_UUID128_INIT(
        0xF1, 0xDE, 0xBC, 0x9A,
        0x78, 0x56,
        0x34, 0x12,
        0x78, 0x56,
        0x34, 0x12,
        0x78, 0x56,
        0x34, 0x12);

static const ble_uuid128_t medlink_tx_uuid =
    BLE_UUID128_INIT(
        0xF2, 0xDE, 0xBC, 0x9A,
        0x78, 0x56,
        0x34, 0x12,
        0x78, 0x56,
        0x34, 0x12,
        0x78, 0x56, 0x34, 0x12);

static const char *TAG = "MEDLINK_SERVER";

static uint8_t rx_value[100];

static uint16_t rx_handle;
static uint16_t tx_handle;

void medlink_notify(const char *message);

static uint16_t current_conn_handle = BLE_HS_CONN_HANDLE_NONE;

static int medlink_access_cb(uint16_t conn_handle,
                             uint16_t attr_handle,
                             struct ble_gatt_access_ctxt *ctxt,
                             void *arg)
{
    switch (ctxt->op)
    {
        case BLE_GATT_ACCESS_OP_WRITE_CHR:
        {
            uint8_t packet[20];
            uint16_t packet_len = 0;

            int rc = ble_hs_mbuf_to_flat(
                ctxt->om,
                packet,
                sizeof(packet),
                &packet_len);

            if (rc != 0)
            {
                ESP_LOGE(TAG, "Failed to decode packet");
                return BLE_ATT_ERR_UNLIKELY;
            }

            current_conn_handle = conn_handle;

            // Pass packet to protocol layer
            medlink_process_packet(packet, packet_len);

            return 0;
        }

        case BLE_GATT_ACCESS_OP_READ_CHR:
        {
            const char *msg = "Hello Phone";

            os_mbuf_append(ctxt->om,
                           msg,
                           strlen(msg));

            return 0;
        }

        default:
            return 0;
    }
}

void medlink_notify(const char *message)
{
    if (current_conn_handle == BLE_HS_CONN_HANDLE_NONE)
    {
        ESP_LOGW(TAG, "No phone connected");
        return;
    }

    struct os_mbuf *om =
        ble_hs_mbuf_from_flat(
            message,
            strlen(message));
    
    if (om == NULL)
    {
        ESP_LOGE(TAG, "Failed to allocate notification buffer");
        return;
    }

    int rc = ble_gatts_notify_custom(
            current_conn_handle,
            tx_handle,
            om);

    if (rc != 0)
        {
            ESP_LOGE(TAG, "Notification failed (%d)", rc);
        }
    else
        {
            ESP_LOGI(TAG, "Notification sent");
            ESP_LOGI(TAG, "Notification sent: %s", message);
        }
}

static const struct ble_gatt_svc_def medlink_svcs[] =
{
    {
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = &medlink_service_uuid.u,

        .characteristics = (struct ble_gatt_chr_def[])
        {
            {
                .uuid = &medlink_rx_uuid.u,
                .access_cb = medlink_access_cb,
                .flags = BLE_GATT_CHR_F_WRITE,
                .val_handle = &rx_handle,
            },

            {
                .uuid = &medlink_tx_uuid.u,
                .access_cb = medlink_access_cb,
                .flags = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_NOTIFY,
                .val_handle = &tx_handle,
            },

            {
                0,
            }
        },
    },

    {
        0,
    }
};

static void gatt_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg)
{
    switch (ctxt->op)
    {
    case BLE_GATT_REGISTER_OP_SVC:
        ESP_LOGI(TAG, "Service registered");
        break;

    case BLE_GATT_REGISTER_OP_CHR:
        ESP_LOGI(TAG,
             "Characteristic registered. Def=%d Value=%d",
             ctxt->chr.def_handle,
             ctxt->chr.val_handle);
        break;
    

    default:
        break;
    }
}

void medlink_service_init(void)
{
    int rc;

    ESP_LOGI(TAG, "Registering MedLink Service");

    ble_hs_cfg.gatts_register_cb = gatt_register_cb;

    rc = ble_gatts_count_cfg(medlink_svcs);

    if (rc != 0)
    {
        ESP_LOGE(TAG, "ble_gatts_count_cfg failed (%d)", rc);
        return;
    }

    rc = ble_gatts_add_svcs(medlink_svcs);

    ESP_LOGI(TAG, "ble_gatts_add_svcs rc = %d", rc);

    ESP_LOGI(TAG, "RX Handle = %d", rx_handle);
    ESP_LOGI(TAG, "TX Handle = %d", tx_handle);

    if (rc != 0)
    {
        ESP_LOGE(TAG, "ble_gatts_add_svcs failed (%d)", rc);
        return;
    }

    ESP_LOGI(TAG, "MedLink Service Registered");
}


