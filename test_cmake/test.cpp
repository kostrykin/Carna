#include <LibCarna/Version.h>
#include <LibCarna/base/Node.h>
#include <iostream>


int main()
{
    /* Instantiate an object that is implemented in the shared library, to test linking.
     */
    LibCarna::base::Node root;

    /* Write the version number for verification.
     */
    std::cout << LIBCARNA_MAJOR_VERSION << "." << LIBCARNA_MINOR_VERSION << "." << LIBCARNA_PATCH_VERSION << std::endl;
}
