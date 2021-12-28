#pragma once

#include "DataWindow.h"

class ArrayDataWindow : public DataWindow {
public:
	ArrayDataWindow(CKDataArray* array);
	void Process() override;
	void Render() override;
	const char* GetObjectName() override;
private:
	CKDataArray* array;
	int key_col;
	int col_cnt;
	int row_cnt;
};