// standard library for input/output
#include <stdio.h>
// rpi pico standard library
#include "pico/stdlib.h"
#include <vector>

bool check_equality(std::vector<uint> input_vector) {
    if(input_vector.size()<=1) {
        return true;
    }
    for(const uint comp : input_vector) {
        if(input_vector[0] != comp) {
            return false;
        }

    }
    return true;
}

int main() {
    uint BUTTON_PINS[] = {0,1,2,3,4,5,6,7,8,9};
    uint RED_PIN = 10;
    uint GRN_PIN = 11;

    gpio_init(RED_PIN);
    gpio_init(GRN_PIN);
    gpio_set_dir(RED_PIN,GPIO_OUT);
    gpio_set_dir(GRN_PIN,GPIO_OUT);
    
    for(const uint B_PIN : BUTTON_PINS) {
        gpio_init(B_PIN);
        gpio_set_dir(B_PIN,GPIO_IN);
        gpio_pull_down(B_PIN);
    }

    std::vector<uint> pin_code = {3,6,2,8};
    std::vector<uint> input = {};

    bool armed = false;
    gpio_put(RED_PIN,1);
    while(true){
        if(armed == false) {
            // get button presses and assign to input
            for(const uint B_PIN : BUTTON_PINS) {
                bool button = gpio_get(B_PIN);
                if(button == true) {
                    input.push_back(B_PIN);
                    // attempt at debounce
                    sleep_ms(200);
                }
            }
            // once 4 digits have been input, check if correct
            if(input.size() == 4) {
                if(input == pin_code) {
                    gpio_put(RED_PIN,0);
                    gpio_put(GRN_PIN,1);
                    armed = true;

                }
                // incorrect sequence
                else{
                    for(int i = 0; i < 3; i++) {
                        gpio_put(RED_PIN,0);
                        sleep_ms(166);
                        gpio_put(RED_PIN,1);
                        sleep_ms(166);
                    }
                }
                input.erase(input.begin(), input.end());
            }
        }
        else {
            // get button presses and assign to input
            for(const uint B_PIN : BUTTON_PINS) {
                bool button = gpio_get(B_PIN);
                if(button == true) {
                    input.push_back(B_PIN);
                    // attempt at debounce
                    sleep_ms(200);
                }
            }
                // once 4 digits have been input, check if digits are all equal
            if(input.size() == 4) {
                bool equality = check_equality(input);
                if(equality == true) {
                    gpio_put(RED_PIN,1);
                    gpio_put(GRN_PIN,0);
                    armed = false;
                }
                // incorrect sequence
                else{
                    for(int i = 0; i < 3; i++) {
                        gpio_put(RED_PIN,1);
                        sleep_ms(166);
                        gpio_put(RED_PIN,0);
                        sleep_ms(166);
                    }
                }
                input.erase(input.begin(), input.end());
            }
        }
    }
}