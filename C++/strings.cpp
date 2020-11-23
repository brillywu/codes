
//copy from autoware.ai/src/autoware/core_perception/gnss_localizer/nodes/nmea2tfpose/nmea2tfpose_core.cpp
std::vector<std::string> spit(const std::string &string)
{
  std::vector<std::string> str_vec_ptr;
  std::string token;
  std::stringstream ss(string);

  while (getline(ss, token, ','))
    str_vec_ptr.push_back(token);

  return str_vec_ptr;
}
