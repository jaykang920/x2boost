// Copyright (c) 2014 Jae-jun Kang
// See the file COPYING for license details.

#include "xpiler.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include "document.hpp"
#include "options.hpp"

#include "boost_formatter.hpp"
#include "xml_handler.hpp"

using namespace std;

namespace fs = boost::filesystem;

namespace xpiler
{
    options xpiler::options;

    xpiler::handler_map_type xpiler::handlers_;
    xpiler::formatter_map_type xpiler::formatters_;

    xpiler::static_initializer xpiler::static_init_;

    xpiler::static_initializer::static_initializer()
    {
        handlers_[".xml"] = handler_ptr(new xml_handler);
        formatters_["boost"] = formatter_ptr(new boost_formatter);
    }

    xpiler::xpiler()
    {
        formatter_ = formatters_[options.spec];
        formatter_->setup();
    }

    void xpiler::process(const string& path)
    {
        if (fs::is_directory(path))
        {
            process_dir(path);
        }
        else if (fs::exists(path))
        {
            process_file(path);
        }
        else
        {
            cout << path << " doesn't exist." << endl;
            error = true;
        }
    }

    void xpiler::process_dir(const fs::path& path)
    {
        cout << "Directory " << fs::canonical(path).string() << endl;

        fs::directory_iterator di(path);
        fs::directory_iterator end;
        for (; di != end; ++di)
        {
            fs::directory_entry& entry = *di;
            fs::path filename = entry.path().filename();

            fs::path pathname = path / filename;
            if (fs::is_directory(entry.status()))
            {
                if (options.recursive)
                {
                    sub_dirs_.push_back(filename.string());
                    process_dir(pathname);
                    sub_dirs_.pop_back();
                }
            }
            else
            {
                process_file(pathname);
            }
        }
    }

    void xpiler::process_file(const fs::path& path)
    {
        fs::path filename = path.filename();
        string extension = path.extension().string();
        fs::path out_dir;
        if (options.out_dir.empty())
        {
            out_dir = path.parent_path();
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
        handler_map_type::iterator it = handlers_.find(extension);
        if (it == handlers_.end() ||
            (!options.forced && formatter_->is_up_to_date(path)))
        {
            return;
        }
        handler_ptr handler = it->second;

        cout << filename.string() << endl;

        document* doc;
        if (!handler->handle(path.string(), &doc))
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

        if (!formatter_->format(doc, out_dir.string()))
        {
            error = true;
        }

        delete doc;
    }
}

// EOF xpiler.cpp
