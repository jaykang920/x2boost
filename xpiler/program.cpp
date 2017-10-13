// Copyright (c) 2014-2017 Jae-jun Kang
// See the file LICENSE for details.

#include "program.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include "definition.hpp"

#include "boost_formatter.hpp"
#include "xml_handler.hpp"

using namespace std;

namespace fs = boost::filesystem;

namespace xpiler
{
    options program::opts;

    program::handler_map_type program::handlers_;
    program::formatter_map_type program::formatters_;

    program::static_initializer program::static_init_;

    program::program()
    {
        error = false;

        formatter_ = formatters_[opts.spec];
        formatter_->setup();
    }

    void program::process(const string& path)
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

    void program::process_dir(const fs::path& path)
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
                if (opts.recursive)
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

    void program::process_file(const fs::path& path)
    {
        fs::path filename = path.filename();
        string extension = path.extension().string();
        fs::path out_dir;
        if (opts.out_dir.empty())
        {
            out_dir = path.parent_path();
        }
        else
        {
            out_dir = opts.out_dir;
            BOOST_FOREACH(string& sub_dir, sub_dirs_)
            {
                out_dir /= sub_dir;
            }
        }

        boost::algorithm::to_lower(extension);
        handler_map_type::iterator it = handlers_.find(extension);
        if (it == handlers_.end() ||
            (!opts.forced && formatter_->is_up_to_date(path)))
        {
            return;
        }
        handler* handler = it->second;

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

    program::static_initializer::static_initializer()
    {
        handlers_[".xml"] = new xml_handler;
        formatters_["boost"] = new boost_formatter;
    }

    program::static_initializer::~static_initializer()
    {
        BOOST_FOREACH(handler_map_type::value_type& pair, handlers_)
        {
            delete pair.second;
        }
        BOOST_FOREACH(formatter_map_type::value_type& pair, formatters_)
        {
            delete pair.second;
        }
    }
}

// EOF program.cpp
