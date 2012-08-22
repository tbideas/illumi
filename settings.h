/* Spaces in your PWA passphrase and SSID MUST be replaced by $ signs 
   So if your key is "I love monkeys", then you need to set 
   it as: "I$love$monkeys" - Thanks!          
*/

/* Define your wifi settings here */
#define SSID "YourWifi"

/* Define your key */
// #define WPA_PASSPHRASE "my$wpa$passphrase"
// #define WEP_PASSWORD "112233445566778899AABBCCDD"

/* Where is the RGB LED connected (must-be PWM pins) */
#define RED 9
#define GREEN 10
#define BLUE 11

/* The Illumi driver board allows the micro to shutdown completely the PWM to save power */
#define LED_SHUTDOWN 8

/* Is the RGB a common-anode (set false if your hardware is common-cathode) */
#define COMMON_ANODE true


