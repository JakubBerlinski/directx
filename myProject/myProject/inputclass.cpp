#include "inputclass.h"

inputClass::inputClass() {

}

inputClass::inputClass(const inputClass& other) {

}

inputClass::~inputClass() {

}

void inputClass::initialize() {
	//initialize all the keys to being released and not pressed
	for(int i=0; i<256; i++) {
		m_keys[i] = false;
	}

	return;
}

void inputClass::keyDown(unsigned int input) {
	//if a key is pressed then save that state in the key array
	m_keys[input] = true;
	return;
}

void inputClass::keyUp(unsigned int input) {
	//if a key is released then clear that state in the key array
	m_keys[input] = false;
	return;
}

bool inputClass::isKeyDown(unsigned int key) {
	//return what state the key is in
	return m_keys[key];
}
