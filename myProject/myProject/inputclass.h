#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

class inputClass {
public:
	inputClass();
	inputClass(const inputClass&);
	~inputClass();

	void initialize();

	void keyDown(unsigned int);
	void keyUp(unsigned int);

	bool isKeyDown(unsigned int);

private:
	bool m_keys[256];
};

#endif