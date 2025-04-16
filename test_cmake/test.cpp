#include <Carna/Version.h>
#include <Carna/base/Node.h>
#include <iostream>


int main()
{
    /* Instantiate an object that is implemented in the shared library, to test linking.
     */
    Carna::base::Node root;

    /* Write the version number for verification.
     */
    std::cout << CARNA_MAJOR_VERSION << "." << CARNA_MINOR_VERSION << "." << CARNA_PATCH_VERSION << std::endl;
}
