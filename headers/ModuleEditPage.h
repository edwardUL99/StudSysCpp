#ifndef MODULE_EDIT_PAGE
#define MODULE_EDIT_PAGE

#include "studsys/Module.h"
#include "Page.h"

namespace ui {
    class ModuleEditPage : public Page {
        private:
            const Module oldModule;
            Module updatedModule;
            void editName();
            void editCredits();
            void changeLecturer();
            bool saveUpdates();
        
        public:
            ModuleEditPage(const Module &module, StudentSystem &system);
            virtual ~ModuleEditPage() = default;
            virtual void show() override;
    };
}

#endif