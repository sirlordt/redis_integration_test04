#include <iostream>

#include <hiredis/hiredis.h>

int main(int argc, char** argv) {

    const char *path = argv[ 0 ];

    std::cout << "Current path: " << path << std::endl;

    // ***** REDIS *****

    redisContext *c;
    redisReply *reply;

    try {

      struct timeval timeout = { 1, 500000 }; // 1.5 seconds
      c = redisConnectWithTimeout("127.0.0.1", 6379, timeout );

      reply = (redisReply *) redisCommand(c,"AUTH %s", "12345678");
      printf("SET: %s\n", reply->str);
      freeReplyObject(reply);

      reply = (redisReply *) redisCommand(c,"SET %s %s", "foo", "hello world");
      printf("SET: %s\n", reply->str);
      freeReplyObject(reply);

      /* Try a GET and two INCR */
      reply = (redisReply *) redisCommand(c,"GET foo");
      printf("GET foo: %s\n", reply->str);
      freeReplyObject(reply);

      /* Disconnects and frees the context */
      redisFree(c);

    }
    catch ( const std::exception &ex ) {

      std::cout << ex.what() << std::endl;

      if ( c ) {

        redisFree(c);

      }

    }
    // ***** REDIS *****

    std::cout << "Press enter to exit" << std::endl;

    while ( getchar() != '\n' );

    return 0;
}
