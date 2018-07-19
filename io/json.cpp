
#include "io/json.h"

#include <fstream>

#include "detail/io/JsonWriter.h"

namespace TM2IN {
    namespace io {
        int exportJSON(string output_file, vector<PolyhedralSurface *> &ts, bool triangulation_out=false) {
            ofstream fout;
            fout.open(output_file);

            TM2IN::detail::io::JsonWriter writer(fout, triangulation_out);
            writer.write(ts);
            fout.close();
            return 0;
        }
    }
}