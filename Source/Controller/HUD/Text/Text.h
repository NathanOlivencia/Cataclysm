#ifndef _TEXT_H
#define _TEXT_H

	sfText* InitText(char* message, sfVector2f position, sfColor color, int size);
	void UpdateTextDisplay(sfText* text, int dataTribe);
	void UpdateTextsDisplay(sfText** text, int dataTribe[], int index);
	void UpdateTextWithStringDisplay(sfText* text, char* msg, int data);
	void UpdateTextWithSeparator(sfText* text, int data1, char* separator, int data2);
#endif // !_TEXT_H