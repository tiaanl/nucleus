
#include <gtest/gtest.h>

#include "nucleus/utils/observer_list.h"

namespace nu {

struct Foo {
  virtual ~Foo() {}
  virtual void observe(int x) = 0;
};

struct Adder : public Foo {
  int total{0};
  int scaler{0};

  explicit Adder(int scaler) : scaler(scaler) {}

  ~Adder() override {}

  void observe(int x) override { total += x * scaler; }
};

struct Disruptor : public Foo {
  ObserverList<Foo>* list;
  Foo* doomed;

  Disruptor(ObserverList<Foo>* list, Foo* doomed) : list(list), doomed(doomed) {}

  ~Disruptor() override {}

  void observe(int x) override { list->removeObserver(doomed); }
};

struct AddInObserve : public Foo {
  bool added{false};
  ObserverList<Foo>* observerList;
  Adder adder;

  explicit AddInObserve(ObserverList<Foo>* observerList) : observerList(observerList), adder(1) {}

  ~AddInObserve() override {}

  void observe(int x) override {
    if (!added) {
      added = true;
      observerList->addObserver(&adder);
    }
  }
};

TEST(ObserverListTest, Basic) {
  ObserverList<Foo> observerList;
  Adder a{1}, b{-1}, c{1}, d{-1}, e{-1};
  Disruptor evil{&observerList, &c};

  observerList.addObserver(&a);
  observerList.addObserver(&b);

  EXPECT_TRUE(observerList.hasObserver(&a));
  EXPECT_FALSE(observerList.hasObserver(&c));

  FOR_EACH_OBSERVER(Foo, observerList, observe(10));

  observerList.addObserver(&evil);
  observerList.addObserver(&c);
  observerList.addObserver(&d);

  // Removing an observer not in the list should do nothing.
  observerList.removeObserver(&e);

  FOR_EACH_OBSERVER(Foo, observerList, observe(10));

  EXPECT_EQ(20, a.total);
  EXPECT_EQ(-20, b.total);
  EXPECT_EQ(0, c.total);
  EXPECT_EQ(-10, d.total);
  EXPECT_EQ(0, e.total);
}

}  // namespace nu
