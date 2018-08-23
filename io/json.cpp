
#include <fstream>

#include "detail/io/JsonWriter.h"

#include "io/json.h"

namespace TM2IN {
    namespace io {
        int exportRoomBoundaryJSON(string output_file, vector<Room *> &rooms, int boundary_mode) {
            ofstream fout;
            fout.open(output_file);

            TM2IN::detail::io::JsonWriter writer(fout);
            writer.write(rooms, boundary_mode);
            fout.close();
            return 0;
        }
    }
}