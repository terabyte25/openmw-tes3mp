#include <iostream>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include <components/files/configurationmanager.hpp>

#include "jsonimporter.hpp"


namespace bpo = boost::program_options;
namespace bfs = boost::filesystem;



int main(int argc, char** argv)
{
    try
    {
        bpo::options_description desc("Syntax: omwsave-jsonimporter <options> infile.omwsave outfile.json\nAllowed options");
        bpo::positional_options_description p_desc;
        desc.add_options()
            ("help,h", "produce help message")
            ("openmwsave,m", bpo::value<std::string>(), "morrowind .ess save file")
            ("output,o", bpo::value<std::string>(), "output file (.json)")
        ;
        p_desc.add("mwsave", 1).add("output", 1);

        bpo::variables_map variables;

        bpo::parsed_options parsed = bpo::command_line_parser(argc, argv)
            .options(desc)
            .positional(p_desc)
            .run();

        bpo::store(parsed, variables);

        if(variables.count("help") || !variables.count("openmwsave") || !variables.count("output")) {
            std::cout << desc;
            return 0;
        }

        bpo::notify(variables);

        Files::ConfigurationManager cfgManager(true);
        cfgManager.readConfiguration(variables, desc);

        std::string omwsaveFile = variables["openmwsave"].as<std::string>();
        std::string outputFile = variables["output"].as<std::string>();

        const std::string& ext = ".json";
        if (boost::filesystem::exists(boost::filesystem::path(outputFile))
                && (outputFile.size() < ext.size() || outputFile.substr(outputFile.size()-ext.size()) != ext))
        {
            throw std::runtime_error("Output file already exists and does not end in .json.");
        }
        jsonimporter importer(omwsaveFile, outputFile);
        importer.convert();
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}