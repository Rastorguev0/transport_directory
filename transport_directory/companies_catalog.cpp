#include "companies_catalog.h"
#include "utils.h"

#include <algorithm>

using namespace std;

YellowPages::Phone_Type PhoneTypeFromString(const string& str) {
  if (str == "PHONE") return YellowPages::Phone_Type_PHONE;
  else if (str == "FAX") return YellowPages::Phone_Type_FAX;
  else throw runtime_error("Unknown phone type");
}

YellowPages::Name_Type NameTypeFromString(const string& str) {
  if (str == "MAIN") return YellowPages::Name_Type_MAIN;
  else if (str == "SHORT") return YellowPages::Name_Type_SHORT;
  else if (str == "SYNONYM") return YellowPages::Name_Type_SYNONYM;
  else throw runtime_error("Unknown name type");
}

YellowPages::WorkingTimeInterval_Day DayTypeFromString(const std::string& str) {
  if (str == "EVERYDAY")  return YellowPages::WorkingTimeInterval_Day_EVERYDAY;
  else if (str == "MONDAY")  return YellowPages::WorkingTimeInterval_Day_MONDAY;
  else if (str == "TUESDAY")  return YellowPages::WorkingTimeInterval_Day_TUESDAY;
  else if (str == "WEDNESDAY")  return YellowPages::WorkingTimeInterval_Day_WEDNESDAY;
  else if (str == "THURSDAY")  return YellowPages::WorkingTimeInterval_Day_THURSDAY;
  else if (str == "FRIDAY")  return YellowPages::WorkingTimeInterval_Day_FRIDAY;
  else if (str == "SATURDAY")  return YellowPages::WorkingTimeInterval_Day_SATURDAY;
  else if (str == "SUNDAY")  return YellowPages::WorkingTimeInterval_Day_SUNDAY;
  else throw runtime_error("Unknown day type");
}

YellowPages::Name ReadName(const Json::Dict& properties) {
  YellowPages::Name protoname;
  protoname.set_value(properties.at("value").AsString());
  if (properties.count("type"))
    protoname.set_type(NameTypeFromString(properties.at("type").AsString()));
  return protoname;
}

YellowPages::Phone ReadPhone(const Json::Dict& properties) {
  YellowPages::Phone protophone;
  if (properties.count("type")) {
    protophone.set_type(PhoneTypeFromString(properties.at("type").AsString()));
  }
  if (properties.count("country_code")) {
    protophone.set_country_code(properties.at("country_code").AsString());
  }
  if (properties.count("local_code")) {
    protophone.set_local_code(properties.at("local_code").AsString());
  }
  if (properties.count("number")) {
    protophone.set_number(properties.at("number").AsString());
  }
  if (properties.count("extension")) {
    protophone.set_extension(properties.at("extension").AsString());
  }
  return protophone;
}

YellowPages::Url ReadUrl(const Json::Dict& properties) {
  YellowPages::Url protourl;
  protourl.set_value(properties.at("value").AsString());
  return protourl;
}

SphereProto::Coords ReadCoords(const Json::Dict& properties) {
  SphereProto::Coords protocoords;
  if (properties.count("lat")) {
    protocoords.set_lat(stod(properties.at("lat").AsString()));
  }
  if (properties.count("lon")) {
    protocoords.set_lon(stod(properties.at("lon").AsString()));
  }
  return protocoords;
}

YellowPages::Address ReadAddress(const Json::Dict& properties) {
  YellowPages::Address protoaddress;
  if (properties.count("coords")) {
    *(protoaddress.mutable_coords()) = ReadCoords(properties.at("coords").AsMap());
  }
  return protoaddress;
}

YellowPages::NearbyStop ReadNearbyStops(const Json::Dict& properties) {
  YellowPages::NearbyStop protonstops;
  if (properties.count("name")) {
    protonstops.set_name(properties.at("name").AsString());
  }
  if (properties.count("meters")) {
    protonstops.set_meters(properties.at("meters").AsInt());
  }
  return protonstops;
}

YellowPages::WorkingTimeInterval ReadTimeInterval(const Json::Dict& properties) {
  YellowPages::WorkingTimeInterval result;
  result.set_day(DayTypeFromString(properties.at("day").AsString()));
  result.set_minutes_from(properties.at("minutes_from").AsInt());
  result.set_minutes_to(properties.at("minutes_to").AsInt());
  return result;
}

YellowPages::Company ReadCompany(const Json::Dict& properties) {
  YellowPages::Company protocompany;
  if (properties.count("address")) {
    *(protocompany.mutable_address()) = ReadAddress(properties.at("address").AsMap());
  }
  if (properties.count("names")) {
    for (const auto& name_json : properties.at("names").AsArray()) {
      (*protocompany.add_names()) = ReadName(name_json.AsMap());
    }
  }
  if (properties.count("phones")) {
    for (const auto& phone_json : properties.at("phones").AsArray()) {
      (*protocompany.add_phones()) = ReadPhone(phone_json.AsMap());
    }
  }
  if (properties.count("urls")) {
    for (const auto& url_json : properties.at("urls").AsArray()) {
      (*protocompany.add_urls()) = ReadUrl(url_json.AsMap());
    }
  }
  if (properties.count("rubrics")) {
    for (const auto& rubric : properties.at("rubrics").AsArray()) {
      protocompany.add_rubrics(rubric.AsInt());
    }
  }
  if (properties.count("nearby_stops")) {
    for (const auto& nstop : properties.at("nearby_stops").AsArray()) {
      *(protocompany.add_nearby_stops()) = ReadNearbyStops(nstop.AsMap());
    }
  }
  if (properties.count("working_time")) {
    if (properties.at("working_time").AsMap().count("intervals")) {
      for (const auto& interval : properties.at("working_time").AsMap().at("intervals").AsArray()) {
        *(protocompany.mutable_working_time()->add_intervals()) = ReadTimeInterval(interval.AsMap());
      }
    }
  }
  return protocompany;
}


namespace CompanyQuery {

  Phone ReadPhone(const Json::Dict& properties) {
    Phone phone;
    phone.phone = ::ReadPhone(properties);
    if (properties.count("type")) {
      phone.has_type = true;
    }
    return phone;
  }

  Company ReadCompany(const Json::Dict& properties) {
    Company result;
    if (properties.count("names")) {
      for (const auto& name : properties.at("names").AsArray()) {
        result.names.push_back(name.AsString());
      }
    }
    if (properties.count("phones")) {
      for (const auto& phone_json : properties.at("phones").AsArray()) {
        result.phones.push_back(ReadPhone(phone_json.AsMap()));
      }
    }
    if (properties.count("urls")) {
      for (const auto& url : properties.at("urls").AsArray()) {
        result.urls.push_back(url.AsString());
      }
    }
    if (properties.count("rubrics")) {
      for (const auto& rubric : properties.at("rubrics").AsArray()) {
        result.rubrics.push_back(rubric.AsString());
      }
    }

    return result;
  }

}

string CompanyMainName(const YellowPages::Company& company) {
  return find_if(company.names().begin(),
    company.names().end(),
    [](const YellowPages::Name& n) { return n.type() == YellowPages::Name_Type_MAIN; })->value();
}


CompaniesCatalog::CompaniesCatalog(const Json::Dict& rubrics_json,
  const vector<Json::Node>& companies_json)
{
  for (const auto& [number, names_dict] : rubrics_json) {
    rubrics_mapping_[std::stoi(number)] = names_dict.AsMap().at("name").AsString();
  }
  companies_.reserve(companies_json.size());
  for (const auto& company_json : companies_json) {
    companies_.push_back(ReadCompany(company_json.AsMap()));
  }
  ComputeWorkingTime();
}

void CompaniesCatalog::Distribute(const YellowPages::Company* company) {
  for (const auto& name : company->names()) {
    by_names[name.value()].insert(company);
  }
  for (const auto& phone : company->phones()) {
    by_phone_numbers[phone.number()].insert(company);
  }
  for (const auto rubric : company->rubrics()) {
    by_rubrics[rubrics_mapping_.at(rubric)].insert(company);
  }
  for (const auto& url : company->urls()) {
    by_urls[url.value()].insert(company);
  }
}

void CompaniesCatalog::ComputeWorkingTime() {
  for (const auto& company : companies_) {
    auto company_name = CompanyMainName(company);
    if (company.working_time().intervals().empty()) {
      working_time[company_name];
    }
    else for (const auto& interval : company.working_time().intervals()) {
      if (interval.day() == YellowPages::WorkingTimeInterval_Day_EVERYDAY) {
        for (int day = 0; day < 7; ++day) {
          working_time[company_name].push_back(ConvertToMinutes(day, 0, interval.minutes_from()));
          working_time[company_name].push_back(ConvertToMinutes(day, 0, interval.minutes_to()));
        }
      }
      else {
        working_time[company_name].push_back(
          ConvertToMinutes(interval.day() - 1, 0, interval.minutes_from()));
        working_time[company_name].push_back(
          ConvertToMinutes(interval.day() - 1, 0, interval.minutes_to()));
      }
    }
    sort(begin(working_time.at(company_name)), end(working_time.at(company_name)));
  }
}

bool CompaniesCatalog::DoesPhoneMatch(const CompanyQuery::Phone& query, const YellowPages::Phone& phone) {
  const auto& query_phone = query.phone;
  if (!query_phone.extension().empty() && query_phone.extension() != phone.extension()) {
    return false;
  }
  if (query.has_type && query_phone.type() != phone.type()) {
    return false;
  }
  if (!query_phone.country_code().empty() && query_phone.country_code() != phone.country_code()) {
    return false;
  }
  if (
    (!query_phone.local_code().empty() || !query_phone.country_code().empty())
    && query_phone.local_code() != phone.local_code()
    ) {
    return false;
  }
  return query_phone.number() == phone.number();
}

double CompaniesCatalog::WaitingForOpen(double cur_time, const std::string& company_name) const {
  const auto& timings = working_time.at(company_name);
  if (timings.empty()) return 0.;
  auto it = upper_bound(begin(timings), end(timings), static_cast<int>(cur_time));
  if (it == end(timings)) return static_cast<double>(*begin(timings) + 7 * 24 * 60) - cur_time;
  if (it == begin(timings)) return static_cast<double>(*begin(timings)) - cur_time;
  int index = prev(it) - begin(timings);
  if (index % 2 == 0) return 0.;
  else return *it - cur_time;
}

const std::string& CompaniesCatalog::GetRubric(uint64_t id) const {
  return rubrics_mapping_.at(id);
}

const std::vector<YellowPages::Company>& CompaniesCatalog::GetCompanies() const {
  return companies_;
}


unordered_set<const YellowPages::Company*> CompaniesCatalog::FindCompanies(
  const CompanyQuery::Company& query) const
{
  unordered_set<const YellowPages::Company*> set_result;
  if (!query.names.empty()) {
    unordered_set<const YellowPages::Company*> for_names;
    for (const auto& name : query.names) {
      if (!by_names.count(name)) continue;
      for_names.insert(by_names.at(name).begin(), by_names.at(name).end());
    }
    set_result = for_names;
    if (set_result.empty()) return {};
  }

  if (!query.rubrics.empty()) {
    unordered_set<const YellowPages::Company*> for_rubrics;
    for (const auto& rubric : query.rubrics) {
      if (!by_rubrics.count(rubric)) continue;
      for_rubrics.insert(by_rubrics.at(rubric).begin(), by_rubrics.at(rubric).end());
    }
    if (!set_result.empty()) set_result = Intersect(set_result, for_rubrics);
    else set_result = for_rubrics;
    if (set_result.empty()) return {};
  }

  if (!query.urls.empty()) {
    unordered_set<const YellowPages::Company*> for_urls;
    for (const auto& url : query.urls) {
      if (!by_urls.count(url)) continue;
      for_urls.insert(by_urls.at(url).begin(), by_urls.at(url).end());
    }
    if (!set_result.empty()) set_result = Intersect(set_result, for_urls);
    else set_result = for_urls;
    if (set_result.empty()) return {};
  }

  if (!query.phones.empty()) {
    unordered_set<const YellowPages::Company*> for_phones;
    for (const auto& query_phone : query.phones) {
      if (!by_phone_numbers.count(query_phone.phone.number())) continue;
      for (const auto& company : by_phone_numbers.at(query_phone.phone.number())) {
        for (const auto& phone : company->phones()) {
          if (DoesPhoneMatch(query_phone, phone)) {
            for_phones.insert(company);
            break;
          }
        }
      } 
    }
    if (!set_result.empty()) set_result = Intersect(set_result, for_phones);
    else set_result = for_phones;
    if (set_result.empty()) return {};
  }
  
  return set_result;
}