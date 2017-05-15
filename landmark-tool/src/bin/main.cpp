#include <gtkmm.h>
#include "singleton/GuiDescription.h"
#include "static_initializations.h"
#include "observer/Observatory.h"

int main(int argc, char *argv[])
{
  //  Gtk::Main kit(argc, argv);
  Glib::RefPtr<Gtk::Application> app =
    Gtk::Application::create(argc, argv,
        "org.gtkmm.examples.base");

  // get the builder object
  Glib::RefPtr<Gtk::Builder> builder =
    GuiDescription::get_instance()->get_builder();

  // get main window
  Gtk::Window* window;
  builder->get_widget("mainWindow", window);

  // register observers
  Observatory<OpenScanButtonObserver>::create();
  Observatory<OpenScanDialogObserver>::create();
  Observatory<ImageStackObserver>::create();
  Observatory<DrawingAreaObserver>::create();
  Observatory<ScrollbarObserver>::create();
  Observatory<ViewModeObserver>::create();
  Observatory<SliceObserver>::create();
  Observatory<InternalPlotObserver>::create();
  Observatory<ZoomObserver>::create();
  Observatory<LandmarksObserver>::create();
  Observatory<LandmarkTreeViewObserver>::create();
  Observatory<SaveLandmarksButtonObserver>::create();
  Observatory<SaveLandmarksDialogObserver>::create();
  Observatory<AddLandmarksButtonObserver>::create();
  Observatory<AddLandmarksDialogObserver>::create();
  Observatory<ErrorMessageDialogObserver>::create();
  Observatory<MainWindowObserver>::create();

  // show main window
  return app->run(*window);

}
