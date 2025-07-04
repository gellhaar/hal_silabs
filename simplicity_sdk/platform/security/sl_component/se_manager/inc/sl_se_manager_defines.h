/***************************************************************************//**
 * @file
 * @brief Silicon Labs Secure Engine Manager API definitions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#ifndef SL_SE_MANAGER_DEFINES_H
#define SL_SE_MANAGER_DEFINES_H

#include "sli_se_manager_features.h"

#if defined(SLI_MAILBOX_COMMAND_SUPPORTED) || defined(SLI_VSE_MAILBOX_COMMAND_SUPPORTED)

#if !defined(SLI_SE_MANAGER_HOST_SYSTEM)
#if !defined(SL_TRUSTZONE_NONSECURE)
#if !defined(SE_MANAGER_CONFIG_FILE)
#include "sl_se_manager_config.h"
#else
#include SE_MANAGER_CONFIG_FILE
#endif // SE_MANAGER_CONFIG_FILE
#endif // SL_TRUSTZONE_NONSECURE
#endif // SLI_SE_MANAGER_HOST_SYSTEM

#if defined (SL_COMPONENT_CATALOG_PRESENT)
  #include "sl_component_catalog.h"
#endif

/// @addtogroup sl_se_manager
/// @{

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// Defines

/// @addtogroup sl_se_manager_core
/// @{

/// Context initialization values. Some of the context values are not fully
/// initialized. The user will need to call the corresponding initialization
/// function in order to fully initialize the context objects for further use
/// in the SE Manager API. The purpose of these initialization values is to set
/// the context objects to a known safe state initially when the context object
/// is declared.
#define SL_SE_COMMAND_CONTEXT_INIT           { SLI_SE_MAILBOX_COMMAND_DEFAULT(0), false }

/// @} (end addtogroup sl_se_manager_core)

/// @addtogroup sl_se_manager_util
/// @{

/// Default configuration for OTP initialisation structure.
#if !defined(_SILICON_LABS_32B_SERIES_3)
#if defined(SLI_MAILBOX_COMMAND_SUPPORTED) && (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT)
  #define SL_SE_OTP_INIT_DEFAULT                                \
  {                                                             \
    .enable_secure_boot = false,                                \
    .verify_secure_boot_certificate = false,                    \
    .enable_anti_rollback = false,                              \
    .secure_boot_page_lock_narrow = false,                      \
    .secure_boot_page_lock_full = false,                        \
    .tamper_levels = { 0 },                                     \
    .tamper_filter_period = SL_SE_TAMPER_FILTER_PERIOD_2MIN,    \
    .tamper_filter_threshold = SL_SE_TAMPER_FILTER_THRESHOLD_4, \
    .tamper_flags = 0,                                          \
    .tamper_reset_threshold = 5                                 \
  }
#else
  #define SL_SE_OTP_INIT_DEFAULT             \
  {                                          \
    .enable_secure_boot = false,             \
    .verify_secure_boot_certificate = false, \
    .enable_anti_rollback = false,           \
    .secure_boot_page_lock_narrow = false,   \
    .secure_boot_page_lock_full = false      \
  }
#endif
#else
  #define SL_SE_OTP_INIT_DEFAULT                                \
  {                                                             \
    .enable_secure_boot = false,                                \
    .verify_secure_boot_certificate = false,                    \
    .enable_anti_rollback = false,                              \
    .tamper_levels = { 0 },                                     \
    .tamper_filter_period = SL_SE_TAMPER_FILTER_PERIOD_2MIN,    \
    .tamper_filter_threshold = SL_SE_TAMPER_FILTER_THRESHOLD_4, \
    .tamper_flags = 0,                                          \
    .tamper_reset_threshold = 5                                 \
  }
#endif

/// @} (end addtogroup sl_se_manager_util)

#if defined(SLI_MAILBOX_COMMAND_SUPPORTED)
// -------------------------------
// Defines for SE functionality

/// @addtogroup sl_se_manager_key_handling
/// @{

/// Flags that can be used with asymmetric keys
/// Asymmetric key can only be used for signing (not key exchange)
#define SL_SE_KEY_FLAG_ASYMMETRIC_SIGNING_ONLY (1UL << 10)
/// Described key belongs to a custom ECC domain
#define SL_SE_KEY_FLAG_ASYMMETRIC_USES_CUSTOM_DOMAIN (1UL << 12)
/// Storage buffer contains public part of an asymmetric key
#define SL_SE_KEY_FLAG_ASYMMETRIC_BUFFER_HAS_PUBLIC_KEY (1UL << 13)
/// Storage buffer contains private part of an asymmetric key
#define SL_SE_KEY_FLAG_ASYMMETRIC_BUFFER_HAS_PRIVATE_KEY (1UL << 14)
/// Allow usage of this key by other bus masters or TrustZone levels than the
/// one which created/imported the key
#define SL_SE_KEY_FLAG_ALLOW_ANY_ACCESS (1UL << 15)
/// Old definition. Retained for backwards compatibility.
#define SL_SE_KEY_FLAG_ASYMMMETRIC_SIGNING_ONLY \
  (SL_SE_KEY_FLAG_ASYMMETRIC_SIGNING_ONLY)

#if defined(_SILICON_LABS_32B_SERIES_3)
/// Flags that can be used with symmetric keys
/// Usage of key requires DPA countermeasures
#define SL_SE_KEY_FLAG_SYMMETRIC_KEY_USAGE_REQUIRE_DPA  (1 << 9)
/// Usage of key requires DFA countermeasures
#define SL_SE_KEY_FLAG_SYMMETRIC_KEY_USAGE_REQUIRE_DFA  (1 << 8)
#endif

/// Do not allow exporting the key to plaintext
#define SL_SE_KEY_FLAG_NON_EXPORTABLE (1UL << 24)
/// Indicate that the key has been generated by this device. This flag is only
/// valid when using the SE to generate a key and makes it non-exportable.
#define SL_SE_KEY_FLAG_IS_DEVICE_GENERATED (1UL << 25)
/// Indicate that the key can only be used to sign SE generated content. This
/// flag is only valid when using the SE to generate a key and makes it
/// non-exportable.
#define SL_SE_KEY_FLAG_IS_RESTRICTED (1UL << 25 | 1UL << 24)

/// Mask for algorithm field in key type
#define SL_SE_KEY_TYPE_ALGORITHM_MASK       0xf0000000
/// Offset of algorithm field in key type
#define SL_SE_KEY_TYPE_ALGORITHM_OFFSET     28
/// Mask for attributes field in key type
#define SL_SE_KEY_TYPE_ATTRIBUTES_MASK      0x00007fff
/// Offset of attributes field in key type
#define SL_SE_KEY_TYPE_ATTRIBUTES_OFFSET    0

/// Symmetric key type
#define SL_SE_KEY_TYPE_SYMMETRIC    0x00000000
/// Symmetric key type for AES-128 (16 byte key)
#define SL_SE_KEY_TYPE_AES_128      0x00000010
/// Symmetric key type for AES-192 (24 byte key)
#define SL_SE_KEY_TYPE_AES_192      0x00000018
/// Symmetric key type for AES-256 (32 byte key)
#define SL_SE_KEY_TYPE_AES_256      0x00000020

/// ECC Weierstrass Prime key type
#define SL_SE_KEY_TYPE_ECC_WEIERSTRASS_PRIME_CUSTOM     (0x8U << SL_SE_KEY_TYPE_ALGORITHM_OFFSET)
/// ECC Montgomery key type
#define SL_SE_KEY_TYPE_ECC_MONTGOMERY                   (0xbU << SL_SE_KEY_TYPE_ALGORITHM_OFFSET)
/// EDDSA key type
#define SL_SE_KEY_TYPE_ECC_EDDSA                        (0xcU << SL_SE_KEY_TYPE_ALGORITHM_OFFSET)

/// ECC NIST P-192
#define SL_SE_KEY_TYPE_ECC_P192     (SL_SE_KEY_TYPE_ECC_WEIERSTRASS_PRIME_CUSTOM | (0x18))
/// ECC NIST P-224
#define SL_SE_KEY_TYPE_ECC_P224     (SL_SE_KEY_TYPE_ECC_WEIERSTRASS_PRIME_CUSTOM | (0x1C))
/// ECC NIST P-256
#define SL_SE_KEY_TYPE_ECC_P256     (SL_SE_KEY_TYPE_ECC_WEIERSTRASS_PRIME_CUSTOM | (0x20))

/// ECC Ed25519 key for EdDSA
#define SL_SE_KEY_TYPE_ECC_ED25519  (SL_SE_KEY_TYPE_ECC_EDDSA | (0x20))

/// ECC X25519 key for ECDH
#define SL_SE_KEY_TYPE_ECC_X25519   (SL_SE_KEY_TYPE_ECC_MONTGOMERY | (0x20))

#if (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT)
/// Symmetric key type for ChaCha20
  #define SL_SE_KEY_TYPE_CHACHA20     0x00000020

/// ECC NIST P-384
  #define SL_SE_KEY_TYPE_ECC_P384     (SL_SE_KEY_TYPE_ECC_WEIERSTRASS_PRIME_CUSTOM | (0x30))
/// ECC NIST P-521
  #define SL_SE_KEY_TYPE_ECC_P521     (SL_SE_KEY_TYPE_ECC_WEIERSTRASS_PRIME_CUSTOM | (0x42))

/// ECC X448 key for ECDH
  #define SL_SE_KEY_TYPE_ECC_X448     (SL_SE_KEY_TYPE_ECC_MONTGOMERY | (0x38))

/// ECC Ed448 key for EdDSA
  #define SL_SE_KEY_TYPE_ECC_ED448    (SL_SE_KEY_TYPE_ECC_EDDSA | (0x38))

#endif

/// Key storage methods

/// Key is stored in a plaintext buffer in application memory. Application
/// can save its in-memory buffer to non-volatile memory as needed to
/// provide key persistence.
#define SL_SE_KEY_STORAGE_EXTERNAL_PLAINTEXT 0x00
#if (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT)
/// Key is stored encrypted in application memory. This ensures the key in
/// wrapped form is only usable on a specific device. If the key
/// additionally needs to be prevented from ever being output as plaintext,
/// also set the corresponding permission bit. Application can save its
/// in-memory buffer to non-volatile memory as needed to provide key
/// persistence.
/// Keys stored in this way should use the flag
/// SL_SE_KEY_FLAG_NON_EXPORTABLE unless there is a specific need to access
/// the key value outside the SE.
  #define SL_SE_KEY_STORAGE_EXTERNAL_WRAPPED 0x01
/// Key is stored inside the SE, and will persist until system reset or
/// explicitly deleted.
/// Keys stored in this way should use the flag
/// SL_SE_KEY_FLAG_NON_EXPORTABLE unless there is a specific need to access
/// the key value outside the SE.
  #define SL_SE_KEY_STORAGE_INTERNAL_VOLATILE 0x02
#endif
/// Key is one of the pre-defined keys (pre-loaded or write-once) available
/// in the SE. See documentation for a list of available keys.
#define SL_SE_KEY_STORAGE_INTERNAL_IMMUTABLE 0x03

#if defined(_SILICON_LABS_32B_SERIES_3)
/// Key is stored in the KSURAM, an internal Key Slot RAM.
  #define SL_SE_KEY_STORAGE_INTERNAL_KSU      0x04

/// Available KSU Key slots
  #if defined(_SILICON_LABS_32B_SERIES_3_CONFIG_301)
    #define SL_SE_KSU_MAX_KEY_SLOTS           0x20
  #else
    #define SL_SE_KSU_MAX_KEY_SLOTS           0x40
  #endif
#endif

/// List of available internal SE key slots
#if (_SILICON_LABS_SECURITY_FEATURE == _SILICON_LABS_SECURITY_FEATURE_VAULT)
  #define SL_SE_KEY_SLOT_VOLATILE_0 0x00 ///< Internal volatile slot 0
  #define SL_SE_KEY_SLOT_VOLATILE_1 0x01 ///< Internal volatile slot 1
  #define SL_SE_KEY_SLOT_VOLATILE_2 0x02 ///< Internal volatile slot 2
  #define SL_SE_KEY_SLOT_VOLATILE_3 0x03 ///< Internal volatile slot 3
#endif

#if defined(SLI_SE_SUPPORTS_NVM3_INTERNAL_KEY)
/// Minimum key slot value for internal keys
  #define SL_SE_KEY_SLOT_INTERNAL_MIN                   0xF6
/// Internal NVM3 key
  #define SL_SE_KEY_SLOT_NVM3_KEY                       0xF6
#else
/// Minimum key slot value for internal keys
  #define SL_SE_KEY_SLOT_INTERNAL_MIN                   0xF7
#endif
/// Internal TrustZone root key
#define SL_SE_KEY_SLOT_TRUSTZONE_ROOT_KEY             0xF7
/// Internal immutable application secure debug key
#define SL_SE_KEY_SLOT_APPLICATION_SECURE_DEBUG_KEY   0xF8
/// Internal immutable application AES-128 key (bootloader key)
#define SL_SE_KEY_SLOT_APPLICATION_AES_128_KEY        0xFA
/// Internal immutable application secure boot key
#define SL_SE_KEY_SLOT_APPLICATION_SECURE_BOOT_KEY    0xFC
/// Internal immutable application attestation key
#define SL_SE_KEY_SLOT_APPLICATION_ATTESTATION_KEY    0xFE
/// Internal immutable SE attestation key
#define SL_SE_KEY_SLOT_SE_ATTESTATION_KEY             0xFF

/// Size overhead for wrapped keys
#define SLI_SE_WRAPPED_KEY_OVERHEAD  (12 + 16)
/// @} (end addtogroup sl_se_manager_key_handling)

/// @addtogroup sl_se_manager_key_derivation
/// @{

/// Defines mapping the PBKDF2 PRFs to corresponding sl_se_hash_type_t values.
#define SL_SE_PRF_AES_CMAC_128 SL_SE_HASH_NONE    ///< CMAC-AES-128
#define SL_SE_PRF_HMAC_SHA1 SL_SE_HASH_SHA1       ///< HMAC-SHA-1
#define SL_SE_PRF_HMAC_SHA224 SL_SE_HASH_SHA224   ///< HMAC-SHA-224
#define SL_SE_PRF_HMAC_SHA256 SL_SE_HASH_SHA256   ///< HMAC-SHA-256
#define SL_SE_PRF_HMAC_SHA384 SL_SE_HASH_SHA384   ///< HMAC-SHA-384
#define SL_SE_PRF_HMAC_SHA512 SL_SE_HASH_SHA512   ///< HMAC-SHA-512

/// @} (end addtogroup sl_se_manager_key_derivation)

/// @addtogroup sl_se_manager_util
/// @{

/// SE Challenge size
#define SL_SE_CHALLENGE_SIZE                      16

/// Certificate key size
#define SL_SE_CERT_KEY_SIZE                       64

/// Certificate signature size
#define SL_SE_CERT_SIGN_SIZE                      64

/// Batch ID certificate
#define SL_SE_CERT_BATCH                          0x01
/// SE ID certificate
#define SL_SE_CERT_DEVICE_SE                      0x02
/// Host ID certificate
#define SL_SE_CERT_DEVICE_HOST                    0x03

/// @addtogroup sl_se_manager_util_tamper Tamper options
/// @brief
///   Tamper configuration options. Levels, signals and filter options.
/// @{

// SE tamper signal levels
#define SL_SE_TAMPER_LEVEL_IGNORE                 0  ///< No action taken
#define SL_SE_TAMPER_LEVEL_INTERRUPT              1  ///< Generate interrupt
#define SL_SE_TAMPER_LEVEL_FILTER                 2  ///< Increment filter counter
#define SL_SE_TAMPER_LEVEL_RESET                  4  ///< System reset
#define SL_SE_TAMPER_LEVEL_PERMANENTLY_ERASE_OTP  7  ///< Erase OTP - THIS WILL MAKE THE DEVICE INOPERATIONAL!

// SE tamper signals
#if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_1)

#define SL_SE_TAMPER_SIGNAL_RESERVED_1                  0x0   ///< Reserved tamper signal
#define SL_SE_TAMPER_SIGNAL_FILTER_COUNTER              0x1   ///< Filter counter exceeds threshold
#define SL_SE_TAMPER_SIGNAL_WATCHDOG                    0x2   ///< SE watchdog timeout
#define SL_SE_TAMPER_SIGNAL_RESERVED_2                  0x3   ///< Reserved tamper signal
#define SL_SE_TAMPER_SIGNAL_SE_RAM_CRC                  0x4   ///< SE RAM CRC parity error
#define SL_SE_TAMPER_SIGNAL_SE_HARDFAULT                0x5   ///< SE CPU hardfault
#define SL_SE_TAMPER_SIGNAL_RESERVED_3                  0x6   ///< Reserved tamper signal
#define SL_SE_TAMPER_SIGNAL_SE_SOFTWARE_ASSERTION       0x7   ///< SE software triggers an assert
#define SL_SE_TAMPER_SIGNAL_SE_SECURE_BOOT_FAILED       0x8   ///< Secure boot of SE firmware failed
#define SL_SE_TAMPER_SIGNAL_USER_SECURE_BOOT_FAILED     0x9   ///< Secure boot of user code failed
#define SL_SE_TAMPER_SIGNAL_MAILBOX_AUTHORIZATION_ERROR 0xA   ///< Unauthorised command received over the Mailbox interface
#define SL_SE_TAMPER_SIGNAL_DCI_AUTHORIZATION_ERROR     0xB   ///< Unauthorised command received over the DCI interface
#define SL_SE_TAMPER_SIGNAL_FLASH_INTEGRITY_ERROR       0xC   ///< Flash content couldn't be properly authenticated
#define SL_SE_TAMPER_SIGNAL_RESERVED_4                  0xD   ///< Reserved tamper signal
#define SL_SE_TAMPER_SIGNAL_SELFTEST_FAILED             0xE   ///< Integrity error of internal storage is detected
#define SL_SE_TAMPER_SIGNAL_TRNG_MONITOR                0xF   ///< TRNG monitor detected lack of entropy
#define SL_SE_TAMPER_SIGNAL_PRS0                        0x10  ///< PRS channel 0 asserted
#define SL_SE_TAMPER_SIGNAL_PRS1                        0x11  ///< PRS channel 1 asserted
#define SL_SE_TAMPER_SIGNAL_PRS2                        0x12  ///< PRS channel 2 asserted
#define SL_SE_TAMPER_SIGNAL_PRS3                        0x13  ///< PRS channel 3 asserted
#define SL_SE_TAMPER_SIGNAL_PRS4                        0x14  ///< PRS channel 4 asserted
#define SL_SE_TAMPER_SIGNAL_PRS5                        0x15  ///< PRS channel 5 asserted
#define SL_SE_TAMPER_SIGNAL_PRS6                        0x16  ///< PRS channel 6 asserted
#define SL_SE_TAMPER_SIGNAL_PRS7                        0x17  ///< PRS channel 7 asserted
#define SL_SE_TAMPER_SIGNAL_DECOUPLE_BOD                0x18  ///< Decouple brown-out-detector threshold alert
#define SL_SE_TAMPER_SIGNAL_TEMPERATURE_SENSOR          0x19  ///< On-device temperature sensor detects operation outside datasheet specification
#define SL_SE_TAMPER_SIGNAL_VOLTAGE_GLITCH_FALLING      0x1A  ///< Voltage glitch detector detected falling glitch
#define SL_SE_TAMPER_SIGNAL_VOLTAGE_GLITCH_RISING       0x1B  ///< Voltage glitch detector detected rising glitch
#define SL_SE_TAMPER_SIGNAL_SECURE_LOCK_ERROR           0x1C  ///< Debug lock internal logic check failed
#define SL_SE_TAMPER_SIGNAL_SE_DEBUG_GRANTED            0x1D  ///< SE debug granted
#define SL_SE_TAMPER_SIGNAL_DIGITAL_GLITCH              0x1E  ///< Digital glitch detector detected an event
#define SL_SE_TAMPER_SIGNAL_SE_ICACHE_ERROR             0x1F  ///< SE ICACHE checksum error

#define SL_SE_TAMPER_SIGNAL_NUM_SIGNALS                 0x20  ///< Number of tamper signals
// End S2C1
#elif defined(_SILICON_LABS_32B_SERIES_2)

#define SL_SE_TAMPER_SIGNAL_RESERVED_1                  0x0   ///< Reserved tamper signal
#define SL_SE_TAMPER_SIGNAL_FILTER_COUNTER              0x1   ///< Filter counter exceeds threshold
#define SL_SE_TAMPER_SIGNAL_WATCHDOG                    0x2   ///< SE watchdog timeout
#define SL_SE_TAMPER_SIGNAL_RESERVED_2                  0x3   ///< Reserved tamper signal
#define SL_SE_TAMPER_SIGNAL_SE_RAM_ECC_2                0x4   ///< SE RAM 2-bit ECC error
#define SL_SE_TAMPER_SIGNAL_SE_HARDFAULT                0x5   ///< SE CPU hardfault
#define SL_SE_TAMPER_SIGNAL_RESERVED_3                  0x6   ///< Reserved tamper signal
#define SL_SE_TAMPER_SIGNAL_SE_SOFTWARE_ASSERTION       0x7   ///< SE software triggers an assert
#define SL_SE_TAMPER_SIGNAL_SE_SECURE_BOOT_FAILED       0x8   ///< Secure boot of SE firmware failed
#define SL_SE_TAMPER_SIGNAL_USER_SECURE_BOOT_FAILED     0x9   ///< Secure boot of user code failed
#define SL_SE_TAMPER_SIGNAL_MAILBOX_AUTHORIZATION_ERROR 0xA   ///< Unauthorised command received over the Mailbox interface
#define SL_SE_TAMPER_SIGNAL_DCI_AUTHORIZATION_ERROR     0xB   ///< Unauthorised command received over the DCI interface
#define SL_SE_TAMPER_SIGNAL_FLASH_INTEGRITY_ERROR       0xC   ///< Flash content couldn't be properly authenticated
#define SL_SE_TAMPER_SIGNAL_RESERVED_4                  0xD   ///< Reserved tamper signal
#define SL_SE_TAMPER_SIGNAL_SELFTEST_FAILED             0xE   ///< Integrity error of internal storage is detected
#define SL_SE_TAMPER_SIGNAL_TRNG_MONITOR                0xF   ///< TRNG monitor detected lack of entropy
#define SL_SE_TAMPER_SIGNAL_SECURE_LOCK_ERROR           0x10  ///< Debug lock internal logic check failed
#define SL_SE_TAMPER_ATAMPDET_EMPGD                     0x11  ///< Electromagnetic pulse glitch detector
#define SL_SE_TAMPER_ATAMPDET_SUPGD                     0x12  ///< Supply glitch detector
#define SL_SE_TAMPER_SE_ICACHE_ERROR                    0x13  ///< SE ICache RAM error
#define SL_SE_TAMPER_SIGNAL_SE_RAM_ECC_1                0x14  ///< SE RAM 1-bit ECC error
#define SL_SE_TAMPER_SIGNAL_BOD                         0x15  ///< Brown-out-detector threshold alert
#define SL_SE_TAMPER_SIGNAL_TEMPERATURE_SENSOR          0x16  ///< On-device temperature sensor
#define SL_SE_TAMPER_SIGNAL_DPLL_LOCK_FAIL_LOW          0x17  ///< DPLL lock fail low
#define SL_SE_TAMPER_SIGNAL_DPLL_LOCK_FAIL_HIGH         0x18  ///< DPLL lock fail high
// Only C5 and C9 have ETAMPDET
#if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_5) || defined(_SILICON_LABS_32B_SERIES_2_CONFIG_9)
#define SL_SE_TAMPER_SIGNAL_ETAMPDET                    0x19  ///< External tamper detect
#define SL_SE_TAMPER_SIGNAL_PRS0                        0x1A  ///< PRS channel 0 asserted
#define SL_SE_TAMPER_SIGNAL_PRS1                        0x1B  ///< PRS channel 1 asserted
#define SL_SE_TAMPER_SIGNAL_PRS2                        0x1C  ///< PRS channel 2 asserted
#define SL_SE_TAMPER_SIGNAL_PRS3                        0x1D  ///< PRS channel 3 asserted
#define SL_SE_TAMPER_SIGNAL_PRS4                        0x1E  ///< PRS channel 4 asserted
#define SL_SE_TAMPER_SIGNAL_PRS5                        0x1F  ///< PRS channel 5 asserted
// End ETAMPDET (S2 C5 || C9)
#else
#define SL_SE_TAMPER_SIGNAL_PRS0                        0x19  ///< PRS channel 0 asserted
#define SL_SE_TAMPER_SIGNAL_PRS1                        0x1A  ///< PRS channel 1 asserted
#define SL_SE_TAMPER_SIGNAL_PRS2                        0x1B  ///< PRS channel 2 asserted
#define SL_SE_TAMPER_SIGNAL_PRS3                        0x1C  ///< PRS channel 3 asserted
#define SL_SE_TAMPER_SIGNAL_PRS4                        0x1D  ///< PRS channel 4 asserted
#define SL_SE_TAMPER_SIGNAL_PRS5                        0x1E  ///< PRS channel 5 asserted
#define SL_SE_TAMPER_SIGNAL_PRS6                        0x1F  ///< PRS channel 6 asserted
// End ETAMPDET (S2 !C5 && !C9)
#endif

#define SL_SE_TAMPER_SIGNAL_NUM_SIGNALS                 0x20  ///< Number of tamper signals
// End S2 !C1
#elif defined(_SILICON_LABS_32B_SERIES_3)

#define SL_SE_TAMPER_SIGNAL_RESERVED_1                  0x0   ///< Reserved tamper signal
#define SL_SE_TAMPER_SIGNAL_FILTER_COUNTER              0x1   ///< Filter counter exceeds threshold
#define SL_SE_TAMPER_SIGNAL_WATCHDOG                    0x2   ///< SE watchdog timeout
#define SL_SE_TAMPER_SIGNAL_CRYPTO_ERROR                0x3   ///< Crypto error detected
#define SL_SE_TAMPER_SIGNAL_SE_RAM_ECC_2                0x4   ///< SE RAM 2-bit ECC error
#define SL_SE_TAMPER_SIGNAL_RESERVED_2                  0x5   ///< Reserved tamper signal
#define SL_SE_TAMPER_SIGNAL_SE_MAJOR_FAULT              0x6   ///< SE major fault detected
#define SL_SE_TAMPER_SIGNAL_L2ICACHE                    0x7   ///< L2 instruction cache error
#define SL_SE_TAMPER_SIGNAL_RESERVED_3                  0x8   ///< Reserved tamper signal
#define SL_SE_TAMPER_SIGNAL_USER_SECURE_BOOT_FAILED     0x9   ///< Secure boot of user code failed
#define SL_SE_TAMPER_SIGNAL_MAILBOX_AUTHORIZATION_ERROR 0xA   ///< Unauthorised command received over the Mailbox interface
#define SL_SE_TAMPER_SIGNAL_DCI_AUTHORIZATION_ERROR     0xB   ///< Unauthorised command received over the DCI interface
#define SL_SE_TAMPER_SIGNAL_SE_SOFTWARE_ASSERTION       0xC   ///< SE software triggers an assert
#define SL_SE_TAMPER_SIGNAL_RESERVED_4                  0xD   ///< Reserved tamper signal
#define SL_SE_TAMPER_SIGNAL_SELFTEST_FAILED             0xE   ///< Integrity error of internal storage is detected
#define SL_SE_TAMPER_SIGNAL_TRNG_MONITOR                0xF   ///< TRNG monitor detected lack of entropy
#define SL_SE_TAMPER_SIGNAL_SECURE_LOCK_ERROR           0x10  ///< Debug lock internal logic check failed
#define SL_SE_TAMPER_ATAMPDET                           0x11  ///< Any tamper detection
#define SL_SE_TAMPER_SIGNAL_OTP_ALARM                   0x12  ///< OTP alarm triggered
#define SL_SE_TAMPER_SE_ICACHE_ERROR                    0x13  ///< SE ICache RAM error
#define SL_SE_TAMPER_SIGNAL_SE_RAM_ECC_1                0x14  ///< SE RAM 1-bit ECC error
#define SL_SE_TAMPER_SIGNAL_BOD                         0x15  ///< Brown-out-detector threshold alert
#define SL_SE_TAMPER_SIGNAL_TEMPERATURE_SENSOR          0x16  ///< On-device temperature sensor
#define SL_SE_TAMPER_SIGNAL_DPLL_LOCK_FAIL              0x17  ///< DPLL lock failure
#define SL_SE_TAMPER_SIGNAL_SOC_PLL_FAIL                0x18  ///< SoC PLL failure
#define SL_SE_TAMPER_SIGNAL_ETAMPDET                    0x19  ///< External tamper detect
#define SL_SE_TAMPER_SIGNAL_KSU_ECC_1                   0x1A  ///< KSU ECC 1-bit error
#define SL_SE_TAMPER_SIGNAL_KSU_ECC_2                   0x1B  ///< KSU ECC 2-bit error
#define SL_SE_TAMPER_SIGNAL_QSPI_RESEED_ERR             0x1C  ///< QSPI reseed error
#define SL_SE_TAMPER_SIGNAL_PRS0                        0x1D  ///< PRS channel 0 asserted
#define SL_SE_TAMPER_SIGNAL_PRS1                        0x1E  ///< PRS channel 1 asserted
#define SL_SE_TAMPER_SIGNAL_PRS2                        0x1F  ///< PRS channel 2 asserted

#define SL_SE_TAMPER_SIGNAL_NUM_SIGNALS                 0x20  ///< Number of tamper signals
// End S3
#endif

// SE tamper filter timeout period.
#define SL_SE_TAMPER_FILTER_PERIOD_32MS     0x0   ///< Timeout ~32ms
#define SL_SE_TAMPER_FILTER_PERIOD_64MS     0x1   ///< Timeout ~64ms
#define SL_SE_TAMPER_FILTER_PERIOD_128MS    0x2   ///< Timeout ~128ms
#define SL_SE_TAMPER_FILTER_PERIOD_256MS    0x3   ///< Timeout ~256ms
#define SL_SE_TAMPER_FILTER_PERIOD_512MS    0x4   ///< Timeout ~512ms
#define SL_SE_TAMPER_FILTER_PERIOD_1S       0x5   ///< Timeout ~1s
#define SL_SE_TAMPER_FILTER_PERIOD_2S       0x6   ///< Timeout ~2s
#define SL_SE_TAMPER_FILTER_PERIOD_4S       0x7   ///< Timeout ~4.1s
#define SL_SE_TAMPER_FILTER_PERIOD_8S       0x8   ///< Timeout ~8.2s
#define SL_SE_TAMPER_FILTER_PERIOD_16S      0x9   ///< Timeout ~16.4s
#define SL_SE_TAMPER_FILTER_PERIOD_33S      0xA   ///< Timeout ~32.8s
#define SL_SE_TAMPER_FILTER_PERIOD_1MIN     0xB   ///< Timeout ~1.1min
#define SL_SE_TAMPER_FILTER_PERIOD_2MIN     0xC   ///< Timeout ~2.2min
#define SL_SE_TAMPER_FILTER_PERIOD_4MIN     0xD   ///< Timeout ~4.4min
#define SL_SE_TAMPER_FILTER_PERIOD_9MIN     0xE   ///< Timeout ~8.7min
#define SL_SE_TAMPER_FILTER_PERIOD_18MIN    0xF   ///< Timeout ~17.5min
#define SL_SE_TAMPER_FILTER_PERIOD_35MIN    0x10  ///< Timeout ~35min
#define SL_SE_TAMPER_FILTER_PERIOD_1H       0x11  ///< Timeout ~1.2h
#define SL_SE_TAMPER_FILTER_PERIOD_2H       0x12  ///< Timeout ~2.3h
#define SL_SE_TAMPER_FILTER_PERIOD_5H       0x13  ///< Timeout ~4.7h
#define SL_SE_TAMPER_FILTER_PERIOD_9H       0x14  ///< Timeout ~9.3h
#define SL_SE_TAMPER_FILTER_PERIOD_19H      0x15  ///< Timeout ~18.6h
#define SL_SE_TAMPER_FILTER_PERIOD_2DAYS    0x16  ///< Timeout ~1.6days
#define SL_SE_TAMPER_FILTER_PERIOD_3DAYS    0x17  ///< Timeout ~3.1days
#define SL_SE_TAMPER_FILTER_PERIOD_6DAYS    0x18  ///< Timeout ~6.2days
#define SL_SE_TAMPER_FILTER_PERIOD_12DAYS   0x19  ///< Timeout ~12.4days
#define SL_SE_TAMPER_FILTER_PERIOD_25DAYS   0x1A  ///< Timeout ~24.9days
#define SL_SE_TAMPER_FILTER_PERIOD_50DAYS   0x1B  ///< Timeout ~49.7days
#define SL_SE_TAMPER_FILTER_PERIOD_100DAYS  0x1C  ///< Timeout ~99.4days
#define SL_SE_TAMPER_FILTER_PERIOD_199DAYS  0x1D  ///< Timeout ~198.8days
#define SL_SE_TAMPER_FILTER_PERIOD_398DAYS  0x1E  ///< Timeout ~397.7days
#define SL_SE_TAMPER_FILTER_PERIOD_795DAYS  0x1F  ///< Timeout ~795.4days

// Number of tamper counts to trigger the filter signal.
#define SL_SE_TAMPER_FILTER_THRESHOLD_2     0x7   ///< Counter threshold 2
#define SL_SE_TAMPER_FILTER_THRESHOLD_4     0x6   ///< Counter threshold 4
#define SL_SE_TAMPER_FILTER_THRESHOLD_8     0x5   ///< Counter threshold 8
#define SL_SE_TAMPER_FILTER_THRESHOLD_16    0x4   ///< Counter threshold 16
#define SL_SE_TAMPER_FILTER_THRESHOLD_32    0x3   ///< Counter threshold 32
#define SL_SE_TAMPER_FILTER_THRESHOLD_64    0x2   ///< Counter threshold 64
#define SL_SE_TAMPER_FILTER_THRESHOLD_128   0x1   ///< Counter threshold 128
#define SL_SE_TAMPER_FILTER_THRESHOLD_256   0x0   ///< Counter threshold 256

/// Tamper flags.
#define SL_SE_TAMPER_FLAG_DGLITCH_ALWAYS_ON (1UL << 1) /// Digital glitch detector always on
#define SL_SE_TAMPER_FLAG_KEEP_TAMPER_ALIVE_DURING_SLEEP (1UL << 2) /// Tamper is kept alive during sleep (down to EM3)

/// @} (end addtogroup sl_se_manager_util_tamper)
/// @} (end addtogroup sl_se_manager_util)

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN

#ifdef SL_SUPPRESS_DEPRECATION_WARNINGS_SDK_2025_6
/// Initial values for CMAC streaming context struct @ref sl_se_cmac_multipart_context_t
#define SL_SE_CMAC_STREAMING_INIT_DEFAULT    { NULL, { 0 }, { 0 }, 0 }

/// Initial values for AES-GCM streaming context struct @ref sl_se_gcm_multipart_context_t
#define SL_SE_GCM_STREAMING_INIT_DEFAULT     { NULL, 0, 0, { 0 }, { 0 }, \
                                               { 0 }, 0, 0 }
#else
#define SL_SE_GCM_STREAMING_INIT_DEFAULT _Pragma("GCC warning \"'SL_SE_GCM_STREAMING_INIT_DEFAULT' macro is deprecated as of Simplicity SDK release 2024.12\""){ NULL, 0, 0, { 0 }, { 0 }, \
                                                                                                                                                                 { 0 }, 0, 0 }

#define SL_SE_CMAC_STREAMING_INIT_DEFAULT _Pragma("GCC warning \"'SL_SE_CMAC_STREAMING_INIT_DEFAULT' macro is deprecated as of Simplicity SDK release 2024.12\"") { NULL, { 0 }, { 0 }, 0 }

#endif
/// @endcond

/// @addtogroup sl_se_manager_cipher
/// @{

/// Block size for the AES
#define SL_SE_AES_BLOCK_SIZE   (16u)

/// @} (end addtogroup sl_se_manager_cipher)

/// @addtogroup sl_se_manager_hash
/// @{
#define SL_SE_HASH_STREAMING_INIT_DEFAULT    { NULL, 0, 0, NULL }     ///< Default streaming hash context
#define SL_SE_SHA1_STREAMING_INIT_DEFAULT    { { 0 }, { 0 }, { 0 } }  ///< SHA1 streaming hash context
#define SL_SE_SHA224_STREAMING_INIT_DEFAULT  { { 0 }, { 0 }, { 0 } }  ///< SHA224 streaming hash context
#define SL_SE_SHA256_STREAMING_INIT_DEFAULT  { { 0 }, { 0 }, { 0 } }  ///< SHA256 streaming hash context
#define SL_SE_SHA384_STREAMING_INIT_DEFAULT  { { 0 }, { 0 }, { 0 } }  ///< SHA384 streaming hash context
#define SL_SE_SHA512_STREAMING_INIT_DEFAULT  { { 0 }, { 0 }, { 0 } }  ///< SHA512 streaming hash context
/// @} (end addtogroup sl_se_manager_hash)

#elif defined(SLI_VSE_MAILBOX_COMMAND_SUPPORTED) // defined(SLI_MAILBOX_COMMAND_SUPPORTED)
// -------------------------------
// Defines for Root code functionality

#define SL_SE_ROOT_CONFIG_MCU_SETTINGS_SHIFT                       16U

#endif // defined(SLI_MAILBOX_COMMAND_SUPPORTED)

#if defined(_SILICON_LABS_32B_SERIES_3)
/// @addtogroup sl_se_manager_extmem
/// @{
/// @addtogroup sl_se_memory_region_utils
/// @{
// The maximum number of code regions available on the device.
// The number of available code regions may be different on future devices.
#define SL_SE_MAX_CODE_REGIONS           8
/// @} (end addtogroup sl_se_memory_region_utils)
/// @} (end addtogroup sl_se_manager_extmem)

#endif // defined(_SILICON_LABS_32B_SERIES_3)

#ifdef __cplusplus
}
#endif

/// @} (end addtogroup sl_se)

#endif // defined(SLI_MAILBOX_COMMAND_SUPPORTED) || defined(SLI_VSE_MAILBOX_COMMAND_SUPPORTED)

#endif // SE_MANAGER_DEFINES_H
