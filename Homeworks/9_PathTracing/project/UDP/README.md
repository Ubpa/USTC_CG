# UDP
Ubpa Design Pattern

## Visitor

- `Visitor<Ret([const]void*, Args...)` 
  - `Visitor<Ret(void*, Args...)`: [ncVisitor.h](include/UDP/Visitor/ncVisitor.h) 
  - `Visitor<Ret(const void*, Args...)`: [cVisitor.h](include/UDP/Visitor/ncVisitor.h) 
- `Visitor<Ret(Impl::*)([const]void*, Args...)[const]` 
  - `Visitor<Ret(Impl::*)(void*, Args...)`: [ncincVisitor.h](include/UDP/Visitor/ncincVisitor.h)  
  - `Visitor<Ret(Impl::*)(const void*, Args...)`: [ncicVisitor.h](include/UDP/Visitor/ncicVisitor.h) 
  - `Visitor<Ret(Impl::*)(void*, Args...)const`: [cincVisitor.h](include/UDP/Visitor/cincVisitor.h) 
  - `Visitor<Ret(Impl::*)(const void*, Args...)const`: [cicVisitor.h](include/UDP/Visitor/cicVisitor.h) 

