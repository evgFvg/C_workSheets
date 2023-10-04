#include <stdio.h>
#include <assert.h>
#include <string.h> /*memcmp*/

#include "dhcp.h"


static void TestCreate(void);
static void TestInsert(void);
static void TestRemove(void);
static void StressTest(void);

int main()
{
    TestCreate();
    TestInsert();
    TestRemove();
    StressTest();
    
    printf("All tests have been passed\n");

    return 0;
}

/***************************Test Functions************************************/

static void TestCreate()
{
    unsigned char create_ip[BYTES_IN_IP] = {192, 128, 128, 10};
    dhcp_t *dh = DHCPCreateDHCP(create_ip, 24);
    assert(253 == DHCPCountFree(dh));

    DHCPDestroy(dh);
}

static void TestInsert()
{
    unsigned char res[BYTES_IN_IP] = {0};
    unsigned char ip[BYTES_IN_IP] = {7, 7, 7, 6};
    unsigned char ip_network[BYTES_IN_IP] = {7, 7, 7, 0};
    unsigned char create_ip[BYTES_IN_IP] = {192, 128, 128, 10};

    unsigned char expected_1[BYTES_IN_IP] = {192, 128, 128, 6};
    unsigned char expected_2[BYTES_IN_IP] = {192, 128, 128, 7};
    unsigned char expected_3[BYTES_IN_IP] = {192, 128, 128, 1};
    dhcp_status_t qq = SUCCESS;

    dhcp_t *dh = DHCPCreateDHCP(create_ip, 24);
    assert(253 == DHCPCountFree(dh));

    qq = DHCPAllocateIp(dh, ip, res);
    assert(memcmp(res, expected_1, BYTES_IN_IP) == 0);
    assert(SUCCESS == qq);

    qq = DHCPAllocateIp(dh, ip, res);
    assert(memcmp(res, expected_2, BYTES_IN_IP) == 0);
    assert(SUCCESS == qq);

    qq = DHCPAllocateIp(dh, ip_network, res);
    assert(memcmp(res, expected_3, BYTES_IN_IP) == 0);
    assert(SUCCESS == qq);

    DHCPDestroy(dh);
}

static void TestRemove()
{
    unsigned char res_1[BYTES_IN_IP] = {0};
    unsigned char res_2[BYTES_IN_IP] = {0};
    unsigned char res_3[BYTES_IN_IP] = {0};
    unsigned char untouchible[BYTES_IN_IP] = {192, 128, 128, 0};
    unsigned char not_exist[BYTES_IN_IP] = {192, 128, 128, 50};
    unsigned char ip[BYTES_IN_IP] = {7, 7, 7, 6};
    unsigned char ip_network[BYTES_IN_IP] = {7, 7, 7, 0};
    unsigned char create_ip[BYTES_IN_IP] = {192, 128, 128, 10};
    size_t dhcp_size = 0;

    dhcp_t *dh = DHCPCreateDHCP(create_ip, 24);

    DHCPAllocateIp(dh, ip, res_1);
    DHCPAllocateIp(dh, ip, res_2);
    DHCPAllocateIp(dh, ip_network, res_3);
    dhcp_size = DHCPCountFree(dh);
    assert(250 == dhcp_size);

    assert(SUCCESS == DHCPFreeIp(dh, res_1));
    assert(dhcp_size + 1 == DHCPCountFree(dh));

    assert(INVALID_FREE == DHCPFreeIp(dh, untouchible));/*network_id is ok but IP is secured*/
    assert(dhcp_size + 1 == DHCPCountFree(dh));

    assert(DOUBLE_FREE == DHCPFreeIp(dh, not_exist)); /*network_id is ok but IP doesnt exist*/
    assert(dhcp_size + 1 == DHCPCountFree(dh));


    DHCPDestroy(dh);
}

static void StressTest()
{
    dhcp_status_t qq = SUCCESS;
    unsigned char ip[BYTES_IN_IP] = {0, 0, 0, 0};
    unsigned char res[BYTES_IN_IP] = {0};
    unsigned char create_ip[BYTES_IN_IP] = {192, 128, 128, 10};
    int i = 0;

    dhcp_t *dh = DHCPCreateDHCP(create_ip, 24);

    for (i = 0; i < 253 && SUCCESS == qq; ++i)
    {
        qq = DHCPAllocateIp(dh, ip, res);
    }
    
    assert(i == 253);
    assert(0 == DHCPCountFree(dh));

    DHCPDestroy(dh);
}



