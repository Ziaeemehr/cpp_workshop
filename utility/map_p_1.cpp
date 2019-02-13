// https://www.lonecpluspluscoder.com/2015/08/13/an-elegant-way-to-extract-keys-from-a-c-map/


// An elegant way to extract keys from a C++ map

//  return a vector of all keys in a map
std::vector<std::string> extract_keys(std::map<std::string, std::string> const& input_map) {
    std::vector<std::string> retval;
    for (auto const& element : input_map) {
        retval.push_back(element.first);
    }
    return retval;
}

// return values
std::vector<std::string> extract_values(std::map<std::string, std::string> const& input_map) {
    std::vector<std::string> retval;
    for (auto const& element : input_map) {
        retval.push_back(element.second);
    }
    return retval;
}


// with template

template<typename TK, typename TV>
std::vector<TK> extract_keys(std::map<TK, TV> const& input_map) {
    std::vector<TK> retval;
    for (auto const& element : input_map) {
        retval.push_back(element.first);
    }
    return retval;
}

template<typename TK, typename TV>
std::vector<TV> extract_values(std::map<TK, TV> const& input_map) {
    std::vector<TV> retval;
    for (auto const& element : input_map) {
        retval.push_back(element.second);
    }
    return retval;
}

// with boost library
boost::copy(input_map | boost::adaptors::map_keys,
            std::back_inserter(output_vector));