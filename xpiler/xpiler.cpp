// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "xpiler.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

#include "document.hpp"
#include "options.hpp"

#include "boost_formatter.hpp"
#include "xml_handler.hpp"

using namespace std;
using namespace x2boost;

namespace fs = boost::filesystem;

Options Xpiler::options;

Xpiler::HandlerMapType Xpiler::handlers_;
Xpiler::FormatterMapType Xpiler::formatters_;

Xpiler::StaticInitializer Xpiler::static_init_;

Xpiler::StaticInitializer::StaticInitializer()
{
    handlers_[".xml"] = new XmlHandler();
    formatters_["boost"] = new BoostFormatter();
}

Xpiler::Xpiler()
{
    formatter_ = formatters_[options.spec];
}

void Xpiler::Process(const string& path)
{
    if (fs::is_directory(path))
    {
        ProcessDir(path);
    }
    else if (fs::exists(path))
    {
        ProcessFile(path);
    }
    else
    {
        cout << path << " doesn't exist." << endl;
        error = true;
    }
}

void Xpiler::ProcessDir(const string& path)
{
    cout << "Directory " << fs::canonical(path).string() << endl;

    fs::directory_iterator di(path);
    fs::directory_iterator end;
    for (; di != end; ++di)
    {
        fs::directory_entry& entry = *di;
        fs::path filename = entry.path().filename();

        std::string pathname = (fs::path(path) / filename).string();
        if (fs::is_directory(entry.status()))
        {
            if (options.recursive)
            {
                sub_dirs_.push_back(filename.string());
                ProcessDir(pathname);
                sub_dirs_.pop_back();
            }
        }
        else
        {
            ProcessFile(pathname);
        }
    }
}

void Xpiler::ProcessFile(const string& path)
{
    fs::path p(path);
    fs::path filename = p.filename();
    string extension = p.extension().string();
    fs::path out_dir;
    if (options.out_dir.empty())
    {
        out_dir = p.parent_path();
    }
    else
    {
        out_dir = options.out_dir;
        BOOST_FOREACH(string& sub_dir, sub_dirs_)
        {
            out_dir /= sub_dir;
        }
    }

    boost::algorithm::to_lower(extension);
    HandlerMapType::iterator it = handlers_.find(extension);
    if (it == handlers_.end() /*||
        (!options.forced  and not newer )*/)
    {
        return;
    }
    InputHandler* handler = it->second;

    cout << filename.string() << endl;

    Document* doc;
    if (!handler->Handle(path, &doc))
    {
        error = true;
    }
    if (error == true || doc == NULL)
    {
        return;
    }

    doc->basename = filename.stem().string();

    if (!out_dir.empty() && !fs::is_directory(out_dir))
    {
        fs::create_directories(out_dir);
    }

    if (!formatter_->Format(doc, out_dir.string()))
    {
        error = true;
    }

    delete doc;
}

// EOF xpiler.cpp
