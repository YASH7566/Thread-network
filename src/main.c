#include <zephyr/zephyr.h>
#include <zephyr/drivers/gpio.h>


#include <openthread/thread.h>
#include <openthread/udp.h> 


/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define BUTTON0_NODE DT_NODELABEL(button0)


static void udp_send(void);



void button0_pressed_callback(const struct device *gpiob, struct gpio_callback *cb, gpio_port_pins_t pins){
	udp_send();
}


void main(void)
{	
	static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET(BUTTON0_NODE, gpios);
	if (!(device_is_ready(button0.port))) {printk("port isn't ready\n"); return;}

	gpio_pin_configure_dt(&button0, GPIO_INPUT);
	gpio_pin_interrupt_configure_dt(&button0,GPIO_INT_EDGE_TO_INACTIVE); // configured interrupt
	static struct gpio_callback button0_cb; // will hold information such as the pin number and the function to be called when an interrupt occurs (callback function).
	gpio_init_callback(&button0_cb, button0_pressed_callback, BIT(button0.pin));  /* Initialize the struct gpio_callback button_cb_data by passing this variable,
	along with the callback function and the bit mask for the GPIO pin button.pin to gpio_init_callback().*/
	gpio_add_callback(button0.port, &button0_cb); // add the callback func


	while (1) {
		
		k_msleep(SLEEP_TIME_MS);
	}
}


static void udp_send(void) 
{
	otError error = OT_ERROR_NONE;
	const char *buf = "Hello Thread xd"; 

	otInstance *myInstance; 
	myInstance = openthread_get_default_instance();
	otUdpSocket mySocket; 

	otMessageInfo messageInfo;
	memset(&messageInfo, 0, sizeof(messageInfo)); 
	otIp6AddressFromString("ff03::1", &messageInfo.mPeerAddr);
	messageInfo.mPeerPort = 2222; 

	do{
		error = otUdpOpen(myInstance, &mySocket, NULL, NULL);
		if (error != OT_ERROR_NONE) break;

		otMessage *test_Message = otUdpNewMessage(myInstance, NULL);
		error = otMessageAppend(test_Message, buf, (uint16_t)strlen(buf));
		if (error != OT_ERROR_NONE) break;

		error = otUdpSend(myInstance, &mySocket, test_Message, &messageInfo);
		if (error != OT_ERROR_NONE) break;

		error = otUdpClose(myInstance, &mySocket);
		
	}while(false); 

	if (error == OT_ERROR_NONE)
		printk("Sent.\n");
	else	
		printk("udpSend error: %d\n", error);
}