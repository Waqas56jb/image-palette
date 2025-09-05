#include "lodepng.h"

int main(int argc, const char *argv[]) {

	unsigned width = 0;
	unsigned height = 0;
	std::vector<unsigned char> inpng; // the input file
	std::vector<unsigned char> buffer; // encoded file
	std::vector<unsigned char> image; // the image pixels
	
	lodepng::load_file(inpng, argv[1]); // load the file into inpng
	unsigned insize = inpng.size();
	
	lodepng::State state;
	LodePNGPaletteSettings palette;
	
	lodepng::decode(image, width, height, state, inpng);
	state.encoder.zlibsettings.btype = 0; // means nocompression
	lodepng::encode(buffer, image, width, height, state, palette); // encode the output file to buffer
	
	lodepng::save_file(buffer, "out.png");
	
	return 0;

}