#pragma once

#include "common.h"
#include "formula.h"

class Cell : public CellInterface {
public:
    Cell();
    ~Cell();

    void Set(std::string text);
    void Clear();

    Value GetValue() const override;
    std::string GetText() const override;
    std::vector<Position> GetReferencedCells() const override;

    void InvalidateCache();
private:
    /*========================================*/            /* Базовый тип ячейки */
    class Impl{
    public:
        virtual void Clear() = 0;
        virtual Value GetValue() const = 0;
        virtual std::string GetText() const = 0;
        virtual std::vector<Position> GetReferencedCells() const = 0;
        virtual bool HasCache() const = 0;
        virtual void InvalidateCache() = 0;
        virtual ~Impl() = default;
    };
    /*========================================*/            /* Пустой тип ячейки */
    class EmptyImpl : public Impl{
    public:
        EmptyImpl();
        void Clear() override;
        Value GetValue() const override;
        std::string GetText() const override;
        std::vector<Position> GetReferencedCells() const override;
        bool HasCache() const override;
        virtual void InvalidateCache() override;
    private:
        std::string content_ = "";
    };
    /*========================================*/            /* Текстовый тип ячейки */
    class TextImpl : public Impl{
    public:
        TextImpl(std::string);
        void Clear() override;  
        Value GetValue() const override;
        std::string GetText() const override;
        std::vector<Position> GetReferencedCells() const override;
        bool HasCache() const override;
        virtual void InvalidateCache() override;
    private:
        std::string content_;
    };
    /*========================================*/            /* Формульный тип ячейки */
    class FormulaImpl : public Impl{
    public:
        FormulaImpl(std::string, SheetInterface& sheet);
        void Clear() override; 
        Value GetValue() const override;
        std::string GetText() const override;
        std::vector<Position> GetReferencedCells() const override;
        bool HasCache() const override;
        virtual void InvalidateCache() override;
    private:
        mutable std::optional<FormulaInterface::Value> cache_;
        std::unique_ptr<FormulaInterface> formula_;
        SheetInterface& sheet_;
    };
    /*========================================*/
    std::unique_ptr<Impl> impl_;
    SheetInterface& sheet_;

    std::set<Cell*> dependent_cells_;
    std::set<Cell*> referenced_cells_;
};