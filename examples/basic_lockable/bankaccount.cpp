#include <vector>
#include <complex>
#include <algorithm>
#include <boost/iterator.hpp>
#include <boost/concept_check.hpp>

template <class foo> 
void my_do_sort(std::vector<foo>& v) 
{
  BOOST_CONCEPT_ASSERT((RandomAccessIterator<foo>));
  std::stable_sort(v.begin(),v.end());
}
int main()
{
  std::vector<std::complex<double> > v;
  v.push_back(std::complex<double>(1,3));
  v.push_back(std::complex<double>(2,4));
  my_do_sort(v);
}
