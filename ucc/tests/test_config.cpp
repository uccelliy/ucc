#include "../ucc/config.h"
#include "../ucc/log.h"
#include <yaml-cpp/yaml.h>

ucc::ConfigVar<int>::ptr g_int_value_config = 
	ucc::Config::Lookup("system.port", (int)8080, "system port");

ucc::ConfigVar<float>::ptr g_int_valuex_config = 
	ucc::Config::Lookup("system.port", (float)8080, "system port");

ucc::ConfigVar<float>::ptr g_float_value_config = 
	ucc::Config::Lookup("system.value", (float)10.2f, "system value");

ucc::ConfigVar<std::vector<int>>::ptr g_int_vec_value_config = 
	ucc::Config::Lookup("system.int_vec", std::vector<int>{1,2}, "system int vec");

ucc::ConfigVar<std::list<int>>::ptr g_int_list_value_config = 
	ucc::Config::Lookup("system.int_list", std::list<int>{1,2}, "system int list");

ucc::ConfigVar<std::set<int>>::ptr g_int_set_value_config = 
	ucc::Config::Lookup("system.int_set", std::set<int>{1,2}, "system int set");

ucc::ConfigVar<std::unordered_set<int>>::ptr g_int_uset_value_config = 
	ucc::Config::Lookup("system.int_uset", std::unordered_set<int>{1,2}, "system int uset");

ucc::ConfigVar<std::map<std::string,int>>::ptr g_str_int_map_value_config = 
	ucc::Config::Lookup("system.str_int_map", std::map<std::string ,int>{{"k",2}}, "system str_int map");

ucc::ConfigVar<std::unordered_map<std::string,int>>::ptr g_str_int_umap_value_config = 
	ucc::Config::Lookup("system.str_int_umap", std::unordered_map<std::string ,int>{{"k",2}}, "system str_int umap");

void print_yaml(const YAML::Node node, int level){
	if(node.IsScalar()){
		UCC_LOG_INFO(UCC_LOG_ROOT())<<std::string(level*4,' ')<<node.Scalar()<< "-"<<node.Type()<<"-"<<level;
	}else if(node.IsNull()){
		UCC_LOG_INFO(UCC_LOG_ROOT())<<std::string(level*4,' ')<<"NULL"<<"-"<<node.Type()<<"-"<<level;
	}else if(node.IsMap()){
		for(auto it = node.begin();it != node.end();it++)
		{
			UCC_LOG_INFO(UCC_LOG_ROOT())<<std::string(level*4,' ')<<it->first<<"-"<<it->second<<"-"<<level;
			print_yaml(it->second,level+1);
		}
	}else if(node.IsSequence()){
		for(size_t i = 0 ;i<node.size();++i)
		{
			UCC_LOG_INFO(UCC_LOG_ROOT())<<std::string(level*4,' ')<<i<<"-"<<node[i].Type()<<"-"<<level;
			print_yaml(node[i],level+1);
		}

	}
}

	
void test_yaml() {
	YAML::Node root = YAML::LoadFile("/home/yinruochen/workspace/ucc/bin/conf/test.yml");
	print_yaml(root,0);
	//UCC_LOG_INFO(UCC_LOG_ROOT())<<root;
}

void test_config(){
	UCC_LOG_INFO(UCC_LOG_ROOT())<<"before:"<<g_int_value_config->getValue();
	UCC_LOG_INFO(UCC_LOG_ROOT())<<"before:"<<g_float_value_config->toString();
#define XX(g_var,name,prefix) \
	{ \
		auto& v = g_var->getValue(); \
		for(auto&i : v){ \
		UCC_LOG_INFO(UCC_LOG_ROOT())<< #prefix" "#name": "<<i; \
		} \
		UCC_LOG_INFO(UCC_LOG_ROOT())<< #prefix" "#name"yaml:" <<g_var->toString(); \
	}

#define XX_M(g_var,name,prefix) \
	{ \
		auto& v = g_var->getValue(); \
		for(auto&i : v){ \
		UCC_LOG_INFO(UCC_LOG_ROOT())<< #prefix" "#name": {"<<i.first<<"-"<<i.second<<"}"; \
		} \
		UCC_LOG_INFO(UCC_LOG_ROOT())<< #prefix" "#name"yaml:" <<g_var->toString(); \
	}

	XX(g_int_vec_value_config,int_vec,before);
	XX(g_int_list_value_config,int_list,before);
	XX(g_int_set_value_config,int_set,before);
	XX(g_int_uset_value_config,int_uset,before);
	XX_M(g_str_int_map_value_config,str_int_map,before);
	XX_M(g_str_int_umap_value_config,str_int_umap,before);

	YAML::Node root = YAML::LoadFile("/home/yinruochen/workspace/ucc/bin/conf/test.yml");
	ucc::Config::LoadFromYaml(root);
	UCC_LOG_INFO(UCC_LOG_ROOT())<<"after:"<<g_int_value_config->getValue();
	UCC_LOG_INFO(UCC_LOG_ROOT())<<"after:"<<g_float_value_config->toString();
	
	XX(g_int_vec_value_config,int_vec,after);
	XX(g_int_list_value_config,int_list,after);
	XX(g_int_set_value_config,int_set,after);
	XX(g_int_uset_value_config,int_uset,after);
	XX_M(g_str_int_map_value_config,str_int_map,after);
	XX_M(g_str_int_umap_value_config,str_int_umap,after);

}

class Person{
public:
	Person(){}
	std::string m_name ;
	int m_age = 0;
	bool m_sex = 0;
	bool operator==(const Person& oth)const{
		return m_name ==oth.m_name && m_age == oth.m_age && m_sex == oth.m_sex;
	}
	std::string toString() const{
		std::stringstream ss;
		ss<<"[Person name = "<<m_name<<"age = "<<m_age<<"sex = "<<m_sex<<"]";
		return ss.str();
	}

};
namespace ucc{
template<>
class LexicalCast<std::string,Person>{
public:
	Person operator()(const std::string& v){
		YAML::Node node = YAML::Load(v);
		Person p;
		p.m_name = node["name"].as<std::string>();
		p.m_age = node["age"].as<int>();
		p.m_sex = node["sex"].as<bool>();
		return p;
	}
};

template<>
class LexicalCast<Person,std::string>{
public:
	std::string operator()(const Person& v){
		YAML::Node node;
		node["name"]= v.m_name;
		node["age"] = v.m_age;
		node["sex"] = v.m_sex;
		std::stringstream ss;
		ss<<node;
		return ss.str();
	}
};



}
ucc::ConfigVar<Person>::ptr g_person = 
	ucc::Config::Lookup("class.person", Person(), "system person");

ucc::ConfigVar<std::map<std::string,Person>>::ptr g_person_map = 
	ucc::Config::Lookup("class.map",std::map<std::string,Person>() , "system person");

void test_class(){
	UCC_LOG_INFO(UCC_LOG_ROOT())<<"before:"<<g_person->getValue().toString()<<"-"<<g_person->toString();


#define XX_PM(g_var,prefix) \
	{ \
		auto m = g_person_map->getValue(); \
		for(auto& i : m){ \
			UCC_LOG_INFO(UCC_LOG_ROOT())<<prefix<<":"<<i.first<<"-"<<i.second.toString(); \
		} \
		UCC_LOG_INFO(UCC_LOG_ROOT())<<prefix<<m.size(); \
	}  
	
	g_person->addListener([](const Person& old_value,const Person& new_value){UCC_LOG_INFO(UCC_LOG_ROOT())<<"Old Value="<<old_value.toString()<<"New Value="<<new_value.toString();
	});
	

XX_PM(g_person_map,"class.map before");

	YAML::Node root = YAML::LoadFile("/home/yinruochen/workspace/ucc/bin/conf/test.yml");
	ucc::Config::LoadFromYaml(root);
	UCC_LOG_INFO(UCC_LOG_ROOT())<<"after:"<<g_person->getValue().toString()<<"-"<<g_person->toString();
	XX_PM(g_person_map,"class.map after");
}

void test_log(){
	static ucc::Logger::ptr system_log = UCC_LOG_NAME("system");
	UCC_LOG_INFO(system_log)<<"hello system"<<std::endl;
	std::cout<<ucc::LogMgr::GetInstance()->toYamlString()<<std::endl;
	YAML::Node root = YAML::LoadFile("/home/yinruochen/workspace/ucc/bin/conf/log.yml");
	ucc::Config::LoadFromYaml(root);
	std::cout<<"------------------------"<<std::endl;
	std::cout<<ucc::LogMgr::GetInstance()->toYamlString()<<std::endl;
	std::cout<<"------------------------"<<std::endl;
	std::cout<<root<<std::endl;
	UCC_LOG_INFO(system_log)<<"hello system"<<std::endl;
	ucc::Config::Visit([](ucc::ConfigVarBase::ptr var){
	UCC_LOG_INFO(UCC_LOG_ROOT())<<"name = "<<var->getName()
						  <<"description = "<<var->getDescription()
						  <<"typename =  "<<var->getTypeName()
						  <<"value = "<< var->toString(); 
	});

}

int main(int argc,char** argv){
	//test_yaml();
	//test_config();
	//test_class();
	test_log();
	return 0;
	
}
