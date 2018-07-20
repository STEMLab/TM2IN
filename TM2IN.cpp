
#include "converter/Converter.h"
#include "Options.h"


using namespace std;

int main(int argc, char * argv[]) {
    try{
        Options options = Options(argc, argv);
        createAndRemoveDir(options.version, options.output_dir);

        Converter converter = Converter(options);
        converter.start();

        converter.run();

        converter.finish();
    }
    catch(const std::runtime_error& e){
        fprintf(stderr, "TM2IN : %s\n", e.what());
        exit(EXIT_FAILURE);
    }
    return 0;
}

