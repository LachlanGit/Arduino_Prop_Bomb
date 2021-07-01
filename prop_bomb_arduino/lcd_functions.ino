//Calcuates width of input text and draws it in the center of specified area
void drawStrMiddle(int x, int y, int w, char* input) {
  u8g2.drawStr(x + ((w - u8g2.getStrWidth(input)) / 2), y, input);
}

void drawGlyphMiddle(int x, int y, int w, int input) {
  u8g2.drawGlyph(x + ((w - u8g2.getMaxCharWidth()) / 2), y, input);
}



//Resets string drawing behaviour
void resetFont() {
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontPosBottom();
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
}
