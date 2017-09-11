#include "core_debug.h"
#include "core_pkbuf.h"

#include "s1ap_build.h"
#include "s1ap_conv.h"

#include "types.h"

#include "testutil.h"
#include "testpacket.h"

#define NUM_OF_TEST_DUPLICATED_ENB 4

static void s1setup_test1(abts_case *tc, void *data)
{
    status_t rv;
    net_sock_t *sock[NUM_OF_TEST_DUPLICATED_ENB];
    pkbuf_t *sendbuf;
    pkbuf_t *recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    s1ap_message_t message;
    int rc;
    int i;

    for (i = 0; i < NUM_OF_TEST_DUPLICATED_ENB; i++)
    {
        sock[i] = tests1ap_enb_connect();
        ABTS_PTR_NOTNULL(tc, sock[i]);
    }

    for (i = 0; i < NUM_OF_TEST_DUPLICATED_ENB; i++)
    {
        rv = tests1ap_build_setup_req(
                &sendbuf, S1ap_ENB_ID_PR_macroENB_ID, 0x54f64);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        rv = tests1ap_enb_send(sock[i], sendbuf);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        rc = tests1ap_enb_read(sock[i], recvbuf);
        ABTS_INT_NEQUAL(tc, 0, rc);

        rv = s1ap_decode_pdu(&message, recvbuf);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        s1ap_free_pdu(&message);
    }

    for (i = 0; i < NUM_OF_TEST_DUPLICATED_ENB; i++)
    {
        rv = tests1ap_enb_close(sock[i]);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);
    }

    pkbuf_free(recvbuf);

    core_sleep(time_from_msec(300));
}

#define NUM_OF_TEST_ENB 32

static void s1setup_test2(abts_case *tc, void *data)
{
    status_t rv;
    net_sock_t *sock[NUM_OF_TEST_ENB];
    pkbuf_t *sendbuf;
    pkbuf_t *recvbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    s1ap_message_t message;
    int rc;
    int i;

    for (i = 0; i < NUM_OF_TEST_ENB; i++)
    {
        sock[i] = tests1ap_enb_connect();
        ABTS_PTR_NOTNULL(tc, sock[i]);
    }

    for (i = 0; i < NUM_OF_TEST_ENB; i++)
    {
        rv = tests1ap_build_setup_req(
                &sendbuf, S1ap_ENB_ID_PR_macroENB_ID, 0x54f64+i);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        rv = tests1ap_enb_send(sock[i], sendbuf);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        rc = tests1ap_enb_read(sock[i], recvbuf);
        ABTS_INT_NEQUAL(tc, 0, rc);

        rv = s1ap_decode_pdu(&message, recvbuf);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);

        s1ap_free_pdu(&message);
    }

    for (i = 0; i < NUM_OF_TEST_ENB; i++)
    {
        rv = tests1ap_enb_close(sock[i]);
        ABTS_INT_EQUAL(tc, CORE_OK, rv);
    }

    pkbuf_free(recvbuf);

    core_sleep(time_from_sec(1));
}

abts_suite *test_s1setup(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, s1setup_test1, NULL);
    abts_run_test(suite, s1setup_test2, NULL);

    return suite;
}
