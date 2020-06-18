//
//  main.cpp
//  UI
//
//  Created by Hao Zhou on 16/06/2020.
//  Copyright © 2020 aiofwa. All rights reserved.
//

#include <iostream>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "Node.hpp"
#include "Bus.hpp"
#define RETINA_MODIFIER 2
#define NODES 3
#define SHAPE 60.0f
#define CAMSPEED 1000.0f


bool intersects(sf::Vector2f pos, sf::Vector2f widget) {
    return !(pos.x < widget.x || pos.x > widget.x + SHAPE || pos.y < widget.y || pos.y > widget.y + SHAPE);
}

int main(int argc, const char * argv[]) {
    sf::RenderWindow window(sf::VideoMode(800 * RETINA_MODIFIER, 600 * RETINA_MODIFIER), "CSMA/CD");
    Bus bus;
    
    sf::Color good(63, 150, 43);
    sf::Color busy(255, 125, 0);
    sf::Color idle(210, 210, 210);
    sf::Color warning(214, 32, 58);
    sf::Clock clock;
    clock.restart();
    
    sf::Font noto;
    if (!noto.loadFromFile("Assets/noto.otf")) {
        return -1;
    }

    std::vector<Node> nodes;
    for (int i = 0; i < NODES; i++) {
        nodes.push_back(Node(&bus));
    }
    
    sf::Text title(sf::String(L"CSMA/CD 实验\n\nWASD - 移动摄像头\nSPACE - 开始/停止\nN - 增加节点\nM - 减少节点\nF - 加速\nR - 减速"), noto);
    title.setPosition(20.0f, 20.0f);
    title.setFillColor(sf::Color::Black);
    
    sf::Vector2u size = window.getSize();
    sf::View view;
    view.setCenter(size.x * 0.5f, size.y * 0.5f);
    view.setSize(window.getSize().x, window.getSize().y);
    
    std::map<sf::Keyboard::Key, bool> keyMap;
    float time = 0.0f;
    float updateInterval = 0.1f;
    
    while (window.isOpen()) {
        sf::Event e;
        size = window.getSize();
        float deltaTime = clock.getElapsedTime().asSeconds();
        clock.restart();
        
        time += deltaTime;
        if (time >= updateInterval) {
            for (int i = 0; i < nodes.size(); i++) {
                nodes[i].update();
            }
            time = 0.0f;
        }
        
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) { window.close(); }
            if (e.type == sf::Event::KeyPressed) {
                keyMap[e.key.code] = true;
                if (e.key.code == sf::Keyboard::F) {
                    updateInterval *= 0.5f;
                } else if (e.key.code == sf::Keyboard::R) {
                    updateInterval *= 2.0f;
                }
            }
            if (e.type == sf::Event::KeyReleased) {
                keyMap[e.key.code] = false;
            }
        }
        if (keyMap[sf::Keyboard::Key::W]) {
            view.setCenter(view.getCenter().x, view.getCenter().y - CAMSPEED * deltaTime);
        }
        if (keyMap[sf::Keyboard::Key::A]) {
            view.setCenter(view.getCenter().x - CAMSPEED * deltaTime, view.getCenter().y);
        }
        if (keyMap[sf::Keyboard::Key::S]) {
            view.setCenter(view.getCenter().x, view.getCenter().y + CAMSPEED * deltaTime);
        }
        if (keyMap[sf::Keyboard::Key::D]) {
            view.setCenter(view.getCenter().x + CAMSPEED * deltaTime, view.getCenter().y);
        }
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        window.clear(sf::Color::White);
        window.draw(title);
        
        window.setView(view);
        float firstX = 0.0f;
        float lastX = 0.0f;
        struct {
            sf::Color color;
            sf::Vector2f pos;
            int index;
            bool shouldDraw;
        } info;
        info.shouldDraw = false;
        for (int i = 0; i < nodes.size(); i++) {
            Node &node = nodes[i];
            sf::Color color;
            switch (node.state) {
                case LISTENING:
                    color = good;
                    break;
                    
                case SENDING:
                    color = busy;
                    break;
                    
                case WARNING:
                    color = warning;
                    break;
                    
                case IDLE:
                    color = idle;
                    
                default:
                    color = idle;
                    break;
            }
            sf::RectangleShape shape;
            shape.setSize({ SHAPE, SHAPE });
            float offset = size.x / 2.0f - (nodes.size() * (shape.getSize().x + SHAPE)) * 0.5f - 100.0f;
            float x = offset + (shape.getSize().x + SHAPE) * i;
            if (i == 0) { firstX = x; }
            if (i == nodes.size() - 1) { lastX = x; }
            shape.setPosition(x, (float) size.y * 0.4f);
            shape.setOutlineColor(color);
            shape.setOutlineThickness(5.0f);
            shape.setFillColor(sf::Color::White);
            
            sf::Text text(std::to_string(i + 1), noto);
            text.setFillColor(color);
            text.setPosition(shape.getPosition().x + 21.0f, shape.getPosition().y + 10.0f);
            
            sf::RectangleShape line;
            line.setPosition(shape.getPosition().x + SHAPE / 2.0f - 2.5f, shape.getPosition().y + shape.getSize().y);
            line.setSize({ 5.0f, 100.0f });
            line.setFillColor(idle);
            
            window.draw(line);
            window.draw(shape);
            window.draw(text);
            
            sf::Vector2f intersectPos = sf::Vector2f(mousePos) + (view.getCenter() - sf::Vector2f(size.x * 0.5f, size.y * 0.5f));
            if (intersects(intersectPos, shape.getPosition())) {
                info.shouldDraw = true;
                info.color = color;
                info.index = i;
                info.pos = intersectPos;
            }
        }
        sf::RectangleShape line;
        line.setPosition(firstX + SHAPE / 2.0f - 2.5f, (float) size.y * 0.4f + SHAPE + 100.0f);
        line.setSize({ lastX - firstX + 200.0f, 5.0f });
        line.setFillColor(idle);
        
        sf::RectangleShape output;
        output.setPosition(line.getPosition().x + line.getSize().x, line.getPosition().y - 30.0f);
        output.setSize({ 130.0f, 60.0f });
        output.setOutlineColor(idle);
        output.setOutlineThickness(5.0f);
        
        std::wstring outputStr;
        switch (bus.channel) {
            case -2: outputStr = L"拥塞"; break;
            case -1: outputStr = L"空闲"; break;
            default: outputStr = std::to_wstring(bus.channel); break;
        }
        sf::Text currentOutput(outputStr, noto);
        currentOutput.setPosition(output.getPosition().x + 21.0f, output.getPosition().y + 10.0f);
        currentOutput.setFillColor(idle);

        window.draw(line);
        window.draw(output);
        window.draw(currentOutput);
        
        if (info.shouldDraw) {
            sf::RectangleShape rect;
            rect.setSize({ 400.0f, 200.0f });
            rect.setPosition(info.pos + sf::Vector2f(10.0f, 10.0f));
            rect.setFillColor(info.color);
            rect.setOutlineThickness(10.0f);
            rect.setOutlineColor(info.color);
            window.draw(rect);
            rect.setFillColor(sf::Color::White);
            rect.setOutlineColor(idle);
            rect.setPosition(info.pos);
            window.draw(rect);
            
            std::wstring state;
            int delay = 0;;
            switch (nodes[info.index].state) {
                case LISTENING:
                    state = L"监听";
                    delay = nodes[info.index].delay;
                    break;
                    
                case SENDING:
                    state = L"发送数据";
                    delay = 256 - nodes[info.index].ptr;
                    break;
                    
                case WARNING:
                    state = L"发送警告";
                    delay = nodes[info.index].delay;
                    break;
                    
                case IDLE:
                    state = L"空闲";
                    delay = nodes[info.index].delay;
                    break;
                    
                default: state = L"不知道"; break;
            }
            sf::Text text(L"节点 " + std::to_wstring(info.index + 1) + L"\n" +
                          L"当前状态: " + state + L"\n" +
                          L"还有 " + std::to_wstring(delay) + L" 比特时间完成\n", noto);
            text.setFillColor(sf::Color::Black);
            text.setPosition(rect.getPosition().x + 20.0f, rect.getPosition().y + 20.0f);
            window.draw(text);
        }
        window.setView(window.getDefaultView());
        window.display();
    }
    return 0;
}
