
#include "io/json.h"

#include <fstream>

#include "detail/io/JsonWriter.h"

namespace TM2IN {
    namespace io {
        int exportJSON(string output_file, vector<PolyhedralSurface *> &ts, bool triangulation_out) {
            ofstream fout;
            fout.open(output_file);

            TM2IN::detail::io::JsonWriter writer(fout);
            writer.write(ts, triangulation_out);
            fout.close();
            return 0;
        }
    }
}