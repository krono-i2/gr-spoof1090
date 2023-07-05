#include <zmq.hpp>
#include <pmt/pmt.h>
#include <unistd.h>
#include "ads_b_net_lib.h"

int main() {

  std::string address = "127.0.0.1";
  std::string port = "5001";

  zmq::context_t context(1);
  zmq::socket_t publisher(context, ZMQ_PUB);
  publisher.bind("tcp://"+address+":"+port);

  //unsigned char even_pkt[15]="\x8d\xab\xcd\xef\x58\x37\x73\xc5\xfa\x4b\xb8\x38\xd7\x8a";
  //unsigned char odd_pkt[15]="\x8d\xab\xcd\xef\x58\x37\x77\xe9\x13\xfa\xf7\x28\x25\x2a";

  unsigned char parity = 0; // Even -> 0 , Odd -> 1

  //cira::aircraft prova("abcdef", 34.6, 43.6, 65.7, parity,"8dabcdef583773c5fa4bb838d78a");
  //cira::aircraft prova("ABCDEF", 9999.0, 56.78, -12.34, parity);
  //prova.print_bytes();

  unsigned char* ads_b_msg = new unsigned char [14];

  float alt = 158.3;
  float lat = -41.39;
  float lon = 11.12; 

  while (1) {
    cira::aircraft* prova = new cira::aircraft("A32DEA", alt, lon, lat, parity);
    cout << alt << endl << lon << endl << lat << endl << endl;
    alt += 0.1;
    lon += 0.01;
    lat += 0.01;
    prova->getMsg(ads_b_msg);
    pmt::pmt_t pdu = pmt::cons(pmt::PMT_NIL,pmt::make_blob(&ads_b_msg[0],14));
    std::string pdu_str(pmt::serialize_str(pdu));
    zmq::message_t msg(pdu_str.length());
    //zmq::message_t msg;
    memcpy(msg.data (), &pdu_str[0], pdu_str.length());
    publisher.send(msg);
    parity ^= 1;
    delete prova;
    usleep(5e5);
  }

  delete ads_b_msg;

  return 0;

}
