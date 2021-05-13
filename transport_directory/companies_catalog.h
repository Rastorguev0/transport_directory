#pragma once
#include "json.h"
#include "database.pb.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace CompanyQuery {

  struct Phone {
    YellowPages::Phone phone;
    bool has_type = false;
  };
  Phone ReadPhone(const Json::Dict& phone_dict);


  struct Company {
    std::vector<std::string> names;
    std::vector<Phone> phones;
    std::vector<std::string> urls;
    std::vector<std::string> rubrics;
  };
  Company ReadCompany(const Json::Dict& properties);

}

YellowPages::Phone_Type PhoneTypeFromString(const std::string& str);
YellowPages::Name_Type NameTypeFromString(const std::string& str);
YellowPages::WorkingTimeInterval_Day DayTypeFromString(const std::string& str);
YellowPages::Name ReadName(const Json::Dict& properties);
YellowPages::Phone ReadPhone(const Json::Dict& properties);
YellowPages::Url ReadUrl(const Json::Dict& properties);
SphereProto::Coords ReadCoords(const Json::Dict& properties);
YellowPages::Address ReadAddress(const Json::Dict& properties);
YellowPages::NearbyStop ReadNearbyStops(const Json::Dict& properties);
YellowPages::WorkingTimeInterval ReadTimeInterval(const Json::Dict& properties);
YellowPages::Company ReadCompany(const Json::Dict& properties);

std::string CompanyMainName(const YellowPages::Company& company);

class CompaniesCatalog {
public:
  CompaniesCatalog(const Json::Dict& rubrics_json, const std::vector<Json::Node>& companies_json);

  YellowPages::Database Serialize() const;
  CompaniesCatalog(const YellowPages::Database& base);

  std::unordered_set<const YellowPages::Company*> FindCompanies(const CompanyQuery::Company&) const;
  static bool DoesPhoneMatch(const CompanyQuery::Phone& query_phone, const YellowPages::Phone& phone);
  double WaitingForOpen(double cur_time, const std::string& company_name) const;
  const std::string& GetRubric(uint64_t id) const;
  const std::vector<YellowPages::Company>& GetCompanies() const;
private:
  using Distribution = std::unordered_map<std::string, std::unordered_set<const YellowPages::Company*>>;
  void Distribute(const YellowPages::Company* company);
  void ComputeWorkingTime();
  
private:
  std::unordered_map<uint64_t, std::string> rubrics_mapping_;
  std::vector<YellowPages::Company> companies_;
  Distribution by_names;
  Distribution by_phone_numbers;
  Distribution by_rubrics;
  Distribution by_urls;
  /*
  каждое число вектора хранит начало временного промежутка,
  соответствующего промежутку работы (четные индексы) и неработы (четные) компании
  число означает минуты, прошедшие с момента наступления первого дня недели.
  */
  std::unordered_map<std::string, std::vector<int>> working_time;
};