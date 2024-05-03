#include <boost/regex.hpp>
#include <iostream>
#include <string>

using namespace std;

/*
 *  usage
 *
 *  Displays the program usage to the user.
 */
void usage(const char *progname)
{
    const char* progname_real; /* contains the real name of the program */
                               /*  (without path) */

    progname_real = strrchr(progname, '/');

    if (progname_real == NULL) /* no path in progname: use progname */
    {
        progname_real = progname;
    }
    else
    {
        progname_real++;
    }

    fprintf(stderr,
            "\nusage: %s < <file> \n\n",
            progname_real);
}

int main(int argc, char *argv[])
{

    int rc=0;

    while ((rc = getopt(argc, argv, "ho:")) != -1)
    {
        switch (rc)
        {
            case 'h':
                usage(argv[0]);
                return 0;
            default:
                fprintf(stderr, "Error: Unknown option '%c'\n", rc);
        }
    }

    std::string line;
    boost::regex pat("^Subject: (Re: |Aw: )*(.*)");

    while (std::cin) {
        std::getline(std::cin, line);
        boost::smatch matches;
        if (boost::regex_match(line, matches, pat))
           std::cout << matches[2] << std::endl;
    }
}