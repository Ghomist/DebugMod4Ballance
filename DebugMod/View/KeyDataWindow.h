#pragma once

#include "DataWindow.h"

#define DEFAULT_KEY_INSPECT		0
#define ARROW_KEY_INSPECT		1
#define ALPHABET_KEY_INSPECT	2
#define FUNCTION_KEY_INSPECT	3
#define EXTENDED_KEY_INSPECT	4
#define ALL_KEY_INSPECT			5

class KeyDataWindow : public DataWindow {
public:
	KeyDataWindow(int _keys);
	void Process() override;
	void Render() override;
	const char* GetObjectName() override;
private:
	int keys_type;
	std::vector<CKKEYBOARD> inspect_keys;
	unsigned char* key_state = nullptr;
	int row_count = 0;
};