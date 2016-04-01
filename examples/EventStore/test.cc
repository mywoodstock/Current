/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Dmitry "Dima" Korolev <dmitry.korolev@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

#include "event_store.h"

// #include "../../3rdparty/gtest/gtest-main-with-dflags.h"
#include "../../3rdparty/gtest/gtest-main.h"

TEST(EventStore, Demo) {
  using event_store_t = EventStore<EventStoreDB<SherlockInMemoryStreamPersister>>;
  using db_t = typename event_store_t::db_t;

  event_store_t event_store;

  const auto add_event_result = event_store.db.Transaction([](MutableFields<db_t> fields) {
    EXPECT_TRUE(fields.events.Empty());
    Event event;
    event.key = "id";
    event.body.some_event_data = "foo";
    fields.events.Add(event);
  }).Go();
  EXPECT_TRUE(WasCommitted(add_event_result));

  const auto verify_event_added_result = event_store.db.Transaction([](ImmutableFields<db_t> fields) {
    EXPECT_EQ(1u, fields.events.Size());
    EXPECT_TRUE(Exists(fields.events["id"]));
    EXPECT_EQ("foo", Value(fields.events["id"]).body.some_event_data);
  }).Go();
  EXPECT_TRUE(WasCommitted(verify_event_added_result));
}
