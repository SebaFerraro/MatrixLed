/**
 * Main file for running the BLE samples.
 */
extern "C" {
	void app_main(void);
}


void MatrixLed(void);


//
// Un-comment ONE of the following
//            ---
void app_main(void) {
	MatrixLed();
} // app_main
