//$$CDS-header$$

#include "tree/hdr_bronson_avltree_map.h"
#include <cds/urcu/signal_threaded.h>
#include <cds/container/bronson_avltree_map_rcu.h>
#include <cds/sync/pool_monitor.h>
#include <cds/memory/vyukov_queue_pool.h>

#include "unit/print_bronsonavltree_stat.h"
#include "unit/print_sync_monitor_stat.h"

namespace tree {
#ifdef CDS_URCU_SIGNAL_HANDLING_ENABLED
    namespace cc = cds::container;
    namespace co = cds::opt;
    namespace {
        typedef cds::urcu::gc< cds::urcu::signal_threaded<> > rcu_type;

        struct print_stat {
            template <typename Tree>
            void operator()( Tree const& t )
            {
                std::cout << t.statistics();
                std::cout << t.monitor().statistics();
            }
        };

        typedef cds::memory::vyukov_queue_pool< std::mutex > simple_pool;
        typedef cds::memory::lazy_vyukov_queue_pool< std::mutex > lazy_pool;
        typedef cds::memory::bounded_vyukov_queue_pool< std::mutex > bounded_pool;
    } // namespace
#endif

    void BronsonAVLTreeHdrTest::BronsonAVLTree_rcu_sht_pool_monitor_less()
    {
#ifdef CDS_URCU_SIGNAL_HANDLING_ENABLED
        struct traits: public
            cc::bronson_avltree::make_traits<
                co::less< std::less<key_type> >
                ,co::sync_monitor< cds::sync::pool_monitor<simple_pool> >
            >::type
        {};
        typedef cc::BronsonAVLTreeMap< rcu_type, key_type, value_type, traits > map_type;
        test<map_type, print_stat>();
#endif
    }

    void BronsonAVLTreeHdrTest::BronsonAVLTree_rcu_sht_pool_monitor_less_stat()
    {
#ifdef CDS_URCU_SIGNAL_HANDLING_ENABLED
        struct traits: public
            cc::bronson_avltree::make_traits<
                co::less< std::less<key_type> >
                ,co::stat< cc::bronson_avltree::stat<> >
                ,co::sync_monitor< cds::sync::pool_monitor<lazy_pool> >
                ,cc::bronson_avltree::relaxed_insert< false >
            >::type
        {};
        typedef cc::BronsonAVLTreeMap< rcu_type, key_type, value_type, traits > map_type;
        test<map_type, print_stat>();
#endif
    }

    void BronsonAVLTreeHdrTest::BronsonAVLTree_rcu_sht_pool_monitor_cmp_ic_stat()
    {
#ifdef CDS_URCU_SIGNAL_HANDLING_ENABLED
        struct traits: public
            cc::bronson_avltree::make_traits<
                co::compare< compare >
                ,co::item_counter< cds::atomicity::item_counter >
                ,co::stat< cc::bronson_avltree::stat<> >
                ,co::sync_monitor< cds::sync::pool_monitor<bounded_pool> >
            >::type
        {};
        typedef cc::BronsonAVLTreeMap< rcu_type, key_type, value_type, traits > map_type;
        test<map_type, print_stat>();
#endif
    }

    void BronsonAVLTreeHdrTest::BronsonAVLTree_rcu_sht_pool_monitor_cmp_ic_stat_yield()
    {
#ifdef CDS_URCU_SIGNAL_HANDLING_ENABLED
        struct traits: public
            cc::bronson_avltree::make_traits<
                co::compare< compare >
                ,co::item_counter< cds::atomicity::item_counter >
                ,co::stat< cc::bronson_avltree::stat<> >
                ,co::back_off< cds::backoff::yield >
                ,co::sync_monitor< cds::sync::pool_monitor<lazy_pool> >
            >::type
        {};
        typedef cc::BronsonAVLTreeMap< rcu_type, key_type, value_type, traits > map_type;
        test<map_type, print_stat>();
#endif
    }

    void BronsonAVLTreeHdrTest::BronsonAVLTree_rcu_sht_pool_monitor_less_relaxed_insert()
    {
#ifdef CDS_URCU_SIGNAL_HANDLING_ENABLED
        struct traits: public
            cc::bronson_avltree::make_traits<
                co::less< std::less<key_type> >
                ,cc::bronson_avltree::relaxed_insert< true >
                ,co::sync_monitor< cds::sync::pool_monitor<lazy_pool> >
            >::type
        {};
        typedef cc::BronsonAVLTreeMap< rcu_type, key_type, value_type, traits > map_type;
        test<map_type, print_stat>();
#endif
    }

    void BronsonAVLTreeHdrTest::BronsonAVLTree_rcu_sht_pool_monitor_less_relaxed_insert_stat()
    {
#ifdef CDS_URCU_SIGNAL_HANDLING_ENABLED
        struct traits: public
            cc::bronson_avltree::make_traits<
                co::less< std::less<key_type> >
                ,co::stat< cc::bronson_avltree::stat<> >
                ,cc::bronson_avltree::relaxed_insert< true >
                ,co::sync_monitor< cds::sync::pool_monitor<simple_pool> >
            >::type
        {};
        typedef cc::BronsonAVLTreeMap< rcu_type, key_type, value_type, traits > map_type;
        test<map_type, print_stat>();
#endif
    }
} // namespace tree
