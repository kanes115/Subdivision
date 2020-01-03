#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/graph_traits_Surface_mesh.h>
#include <CGAL/subdivision_method_3.h>
#include <CGAL/Timer.h>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>
typedef CGAL::Simple_cartesian<double>         Kernel;
typedef CGAL::Surface_mesh<Kernel::Point_3>    PolygonMesh;
using namespace std;
using namespace CGAL;
namespace params = CGAL::parameters;

std::string getMeshDetails(PolygonMesh mesh) {
  return "" + std::to_string(mesh.number_of_vertices()) + ";" + std::to_string(mesh.number_of_faces()) + ";" + std::to_string(mesh.number_of_edges());
}

void doSubdivision(PolygonMesh pmesh, std::string method, int d) {
  auto out_file = method + "_result.off";
  Timer t;

  t.start();
  if(method.compare("Catmull"))
    Subdivision_method_3::CatmullClark_subdivision(pmesh, params::number_of_iterations(d));
  else if(method.compare("DooSabin"))
    Subdivision_method_3::DooSabin_subdivision(pmesh, params::number_of_iterations(d));
  else if(method.compare("Loop"))
    Subdivision_method_3::Loop_subdivision(pmesh, params::number_of_iterations(d));
  else if(method.compare("Sqrt3"))
    Subdivision_method_3::Sqrt3_subdivision(pmesh, params::number_of_iterations(d));
  else {
    std::cerr << "Unknown method" << std::endl;
    return;
  }
  auto time = t.time();
  std::ofstream out(out_file);
  out << pmesh;
  auto mesh_details = getMeshDetails(pmesh);
  std::cout << method << ";" << d << ";" <<  time * 1000 << ";" << mesh_details << std::endl;
}


int main(int argc, char** argv) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " [d] [filename_in] \n";
    cerr << "         d -- the depth of the subdivision \n";
    cerr << "         filename_in -- the input mesh (.off) \n";
    cerr << endl;
    return 1;
  }
  int d = boost::lexical_cast<int>(argv[1]);
  const char* in_file = argv[2];

  PolygonMesh pmesh;
  std::ifstream in(in_file);
  if(in.fail()) {
    std::cerr << "Could not open input file " << in_file << std::endl;
    return 1;
  }
  in >> pmesh;

  std::cout << "method;#iterations;time;#vertices;#faces;#edges" << endl;

  doSubdivision(pmesh, "Catmull", d);
  doSubdivision(pmesh, "DooSabin", d);
  doSubdivision(pmesh, "Loop", d);
  doSubdivision(pmesh, "Sqrt3", d);

  return 0;
}
