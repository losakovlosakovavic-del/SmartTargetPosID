#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/LevelEditorLayer.hpp>

using namespace geode::prelude;

// Переменная для хранения «запомненной» группы
// В реальном моде её можно брать из последнего выделенного объекта
int g_targetGroupId = 0; 
bool g_confirmModeActive = true; // Переключатель работы функции

class $modify(MyEditorUI, EditorUI) {
    
    // Перехватываем касания/клики в редакторе
    bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
        
        // Если функция выключена или группа не задана, работаем как обычно
        if (!g_confirmModeActive || g_targetGroupId <= 0) {
            return EditorUI::ccTouchBegan(touch, event);
        }

        // Получаем точку клика на экране
        auto touchPoint = touch->getLocation();
        
        // Находим объект (триггер), по которому кликнули
        auto* editorLayer = LevelEditorLayer::get();
        if (editorLayer) {
            // Ищем объект под курсором
            auto* clickedObject = editorLayer->getObjectAtPosition(touchPoint);
            
            // Если мы реально кликнули по какому-то объекту/триггеру
            if (clickedObject) {
                
                // Проверяем, есть ли уже эта группа у объекта, чтобы не дублировать
                if (!clickedObject->m_groups->containsObject(cocos2d::CCInteger::create(g_targetGroupId))) {
                    
                    // Добавляем группу объекту
                    clickedObject->addToGroup(g_targetGroupId);
                    
                    // Обновляем визуальное отображение объекта в редакторе
                    clickedObject->updateObject();
                    
                    // Показываем маленькое уведомление на экране, что группа выдана
                    FLAlertLayer::create(
                        "Confirm", 
                        "Группа " + std::to_string(g_targetGroupId) + " успешно добавлена!", 
                        "OK"
                    )->show();
                    
                    // Возвращаем true, чтобы игра поняла, что клик обработан
                    return true; 
                }
            }
        }

        // Если кликнули мимо объекта, выполняем стандартный код Geometry Dash
        return EditorUI::ccTouchBegan(touch, event);
    }
};
