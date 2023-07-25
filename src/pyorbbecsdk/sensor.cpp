#include "sensor.hpp"

#include "error.hpp"

namespace pyorbbecsdk {

void define_sensor(const py::object& m) {
  py::class_<ob::Sensor, std::shared_ptr<ob::Sensor>>(m, "Sensor")
      .def(
          "get_type",
          [](const std::shared_ptr<ob::Sensor>& self) { return self->type(); },
          py::call_guard<py::gil_scoped_release>())
      .def(
          "get_stream_profile_list",
          [](const std::shared_ptr<ob::Sensor>& self) {
            OB_TRY_CATCH({ return self->getStreamProfileList(); });
          },
          py::call_guard<py::gil_scoped_release>())
      .def(
          "start",
          [](const std::shared_ptr<ob::Sensor>& self,
             const std::shared_ptr<ob::StreamProfile>& profile,
             const py::function& callback) {
            OB_TRY_CATCH({
              return self->start(profile,
                                 [callback](std::shared_ptr<ob::Frame> frame) {
                                   py::gil_scoped_acquire acquire;
                                   callback(frame);
                                 });
            });
          },
          py::call_guard<py::gil_scoped_release>())
      .def(
          "stop",
          [](const std::shared_ptr<ob::Sensor>& self) { return self->stop(); },
          py::call_guard<py::gil_scoped_release>())
      .def(
          "switch_profile",
          [](const std::shared_ptr<ob::Sensor>& self,
             const std::shared_ptr<ob::StreamProfile>& profile) {
            OB_TRY_CATCH({ return self->switchProfile(profile); });
          },
          py::call_guard<py::gil_scoped_release>())
      .def(
          "__repr__",
          [](const std::shared_ptr<ob::Sensor>& self) {
            return "<Sensor: " + std::to_string(self->type()) + ">";
          },
          py::call_guard<py::gil_scoped_release>());
}

void define_sensor_list(const py::object& m) {
  py::class_<ob::SensorList, std::shared_ptr<ob::SensorList>>(m, "SensorList")
      .def("get_count",
           [](const std::shared_ptr<ob::SensorList>& self) {
             return self->count();
           })
      .def("get_type_by_index", [](const std::shared_ptr<ob::SensorList>& self,
                                   int index) { return self->type(index); })
      .def("get_sensor_by_index",
           [](const std::shared_ptr<ob::SensorList>& self, int index) {
             return self->getSensor(index);
           })
      .def("get_sensor_by_type",
           [](const std::shared_ptr<ob::SensorList>& self, OBSensorType type) {
             return self->getSensor(type);
           })
      .def("__len__", [](const std::shared_ptr<ob::SensorList>& self) {
        return self->count();
      });
}
}  // namespace pyorbbecsdk
