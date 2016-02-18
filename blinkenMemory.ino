#include <Bounce2.h> // find it here: https://github.com/thomasfredericks/Bounce2
#include <LED.h> // my very own very first very basic arduino library (https://github.com/bubens/LED)

LED led_blink = LED( 6 ); // blinking LED is on pin 6
LED led_ready = LED( 1 ); // LED signaling "ready" is on pin 1
LED led_wait = LED( 2 ); // LED signaling "wait" is on pin 2
LED led_control = LED( 13 ); // onboard control LED (pin 13)

byte button_pin = 7; // button is on pin 7
Bounce button = Bounce(); // unbounced button

int last_press = 0; // holds the time when the button was pressed the last time
int button_counter = 0; // counter for how often the button was pressed
bool button_released = true; // indicates wether the button was released (usefull for some internal unbouncing)


void setup() {
  led_blink.setup(); // same as pinMode( XY, OUTPUT ) but integrated in LED-library
  led_ready.setup();
  led_wait.setup();
  led_control.setup();

  led_ready.on(); // switch the ready led on as soon as possible
  
  pinMode( button_pin, INPUT ); // you get that
  digitalWrite( button_pin, HIGH ); // Internal pull-up... not yet understood
  button.attach( button_pin ); // see Bounce2-documentation
  button.interval( 5 ); // ibid.
}

void loop() {
  button.update(); // see Bounce2-documentation
  byte state = button.read(); // ibid.

  // button is currently not pressed down
  if ( state == LOW ) {
    led_control.off();

    // calculate the time since the last time the button was pressed down
    int t = millis() - last_press;

    // if the button was pressed more than 50ms ago
    // I assume it's a seperate pressing of the button
    if ( t > 50 ) {
      button_released = true;
    }

    // if the last time the button was pressed is more than 500ms ago, we do the blinking
    if ( t > 500 && button_counter > 0 ) {
      // indicate that the user has to wait until the blinking is over
      led_wait.on();
      led_ready.off();

      // do the blinking as often as the button was pressed
      led_blink.blink_n_times( button_counter, 200 );

      // indicate that the user can input again
      led_wait.off();
      led_ready.on();

      // reset the values
      last_press = 0;
      button_counter = 0;
    }
  }

  // the button currently is pressed down AND it was released more than 50ms ago
  // I therefore assume that the current press event is not some bounce artefact
  // but a seperate pressing of the button
  if ( state == HIGH && button_released ) { 
    
    // now remember that the button was pressed; otherwise the program executed the next few
    // lines in every goaround of the loop() while the button is pressed.
    button_released = false; 

    // onboard control led
    led_control.on();

    // increment the counter
    button_counter += 1;

    // remember when the button was pressed
    last_press = millis();
  }
}
