#include <zephyr.h>
#include <device.h>
#include <devicetree.h>

#include <zephyr/zephyr.h>
#include <zephyr/drivers/gpio.h>


#include <net/openthread.h>
#include <openthread/thread.h>
#include <openthread/udp.h> 



void udpReceive_cb(void *aContext, otMessage *aMessage, const otMessageInfo *aMessageInfo);
static void udp_init(void);


void main(void)
{

	// udp_init(); had errors when this this line wasnt in while(1)
	
	while (1) {
		udp_init();
		int a = 5;
		int c=5;
		c = c+5;
		
		// printk("qwerqwr\n") // mcu keeps resetting itself
		// k_msleep(1000); // mcu keeps resetting itself
	}
	

	
}

void udpReceive_cb(void *aContext, otMessage *aMessage, const otMessageInfo *aMessageInfo)
{
	uint16_t payloadLength = otMessageGetLength(aMessage) - otMessageGetOffset(aMessage);
	char buf[payloadLength+1];
	otMessageRead(aMessage, otMessageGetOffset(aMessage),buf, payloadLength);
	buf[payloadLength]='\0';
	printk("Received: %s\n",buf);
}

static void udp_init(void)
{
	otError error = OT_ERROR_NONE;

	otInstance *myInstance = openthread_get_default_instance();

	otUdpSocket mySocket;
	otSockAddr mySockAddr;
	memset(&mySockAddr,0,sizeof(mySockAddr));
	mySockAddr.mPort = 2222; // we chose this port.


	do{
		error = otUdpOpen(myInstance, &mySocket, udpReceive_cb, NULL); //udp mesajı gelince tanımlanan callback funca giriyor.
		if (error != OT_ERROR_NONE) break;
		error = otUdpBind(myInstance, &mySocket, &mySockAddr, OT_NETIF_THREAD);
	}while(false);

	if (error != OT_ERROR_NONE){
		printk("init_udp error: %d\n", error);
	}
}