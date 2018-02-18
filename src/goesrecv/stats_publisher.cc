#include "stats_publisher.h"

#include <cassert>

#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>

std::unique_ptr<StatsPublisher> StatsPublisher::create(const std::string& url) {
  auto fd = Publisher::bind(url.c_str());
  return std::make_unique<StatsPublisher>(fd);
}

StatsPublisher::StatsPublisher(int fd)
    : Publisher(fd) {
}

StatsPublisher::~StatsPublisher() {
}

void StatsPublisher::publish(const std::string& str) {
  if (!hasSubscribers()) {
    return;
  }

  auto rv = nn_send(fd_, str.data(), str.size(), 0);
  if (rv < 0) {
    fprintf(stderr, "nn_send: %s\n", nn_strerror(nn_errno()));
    assert(false);
  }
}
