#include "stdafx.h"

#include <boost/filesystem.hpp>
#include <Poco/Process.h>
#include <Poco/Pipe.h>
#include <Poco/PipeStream.h>
#include <Poco/StreamCopier.h>

namespace fs = boost::filesystem;

TEST(ConsoleTest, HtmlExport)
{
	fs::path console = fs::canonical("../Debug/CppCoverageConsole.exe"); // $$$ improve
	fs::path testExe = fs::canonical("../Debug/ConsoleForCppCoverageTest.exe"); // $$$ improve

	auto tempPath = fs::absolute(fs::unique_path());

	try
	{
		std::vector<std::string> arguments{ testExe.string(), tempPath.string() };
		Poco::Pipe output;
		auto handle = Poco::Process::launch(console.string(), arguments, "../Debug", nullptr, &output, &output);
		handle.wait();

		Poco::PipeInputStream inputStream(output);
		Poco::StreamCopier::copyStream(inputStream, std::cout);
	}
	catch (...)
	{
		fs::remove_all(tempPath);
		throw;
	}

	fs::remove_all(tempPath);
}