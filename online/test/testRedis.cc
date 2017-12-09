 ///
 /// @file    testRedis.cc
 /// @author  mistydew(mistydew@qq.com)
 /// @date    2017-12-09 15:22:30
 ///
 
#include "../deps/cpp_redis/cpp_redis"

#include <iostream>
#include <vector>

int main(void)
{
	cpp_redis::client client;
	client.connect("192.168.40.128", 6379, [](const std::string& host, std::size_t port, cpp_redis::client::connect_state status) {
		if (status == cpp_redis::client::connect_state::dropped) {
			std::cout << "client disconnected from " << host << ":" << port << std::endl;
		}
	});

  client.set("age", "23", [](cpp_redis::reply& reply) {
    std::cout << "set age 22: " << reply << std::endl;
    // if (reply.is_string())
    //   do_something_with_string(reply.as_string())
  });

  client.get("age", [](cpp_redis::reply& reply) {
    std::cout << "get age: " << reply << std::endl;
    // if (reply.is_string())
    //   do_something_with_string(reply.as_string())
  });
  auto get = client.get("age");

	// synchronous commit, no timeout
	client.sync_commit();

	cpp_redis::reply r = get.get();
	std::cout << "reply = " << r << std::endl;
	std::string re = r.as_string();
	  std::cout << "re = " << re << std::endl;
	  std::cout << "r.is_null(): " << r.is_null() << std::endl;
	  if (r.is_null()) {
		  std::cout << "null" << std::endl;
	  } else {
		  std::cout << "!null" << std::endl;
	  }

	return 0;
}
