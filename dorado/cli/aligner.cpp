#include "Version.h"
#include "minimap.h"
#include "utils/bam_utils.h"
#include "utils/log_utils.h"

#include <argparse.hpp>
#include <spdlog/spdlog.h>

#include <string>
#include <vector>

namespace dorado {

int aligner(int argc, char* argv[]) {
    utils::InitLogging();

    argparse::ArgumentParser parser("dorado", DORADO_VERSION, argparse::default_arguments::help);
    parser.add_argument("reads").help("Reads in BAM/SAM/CRAM format.");
    parser.add_argument("-v", "--verbose").default_value(false).implicit_value(true);

    try {
        parser.parse_args(argc, argv);
    } catch (const std::exception& e) {
        std::ostringstream parser_stream;
        parser_stream << parser;
        spdlog::error("{}\n{}", e.what(), parser_stream.str());
        std::exit(1);
    }

    if (parser.get<bool>("--verbose")) {
        spdlog::set_level(spdlog::level::debug);
    }

    auto reads(parser.get<std::string>("reads"));

    utils::BamReader reader(reads);
    utils::BamWriter writer("-", reader.m_header);

    spdlog::info("> input fmt: {} aligned: {}", reader.m_format, reader.m_is_aligned);

    while (reader.next()) {
        writer.write_record(reader.m_record, 16, 0, 0, 50);
    }

    return 0;
}

}  // namespace dorado
