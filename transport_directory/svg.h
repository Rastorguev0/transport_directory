#pragma once
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <memory>

namespace Svg {

	struct Point {
		double x = 0;
		double y = 0;
	};

	struct Rgb {
		uint8_t red;
		uint8_t green;
		uint8_t blue;
	};

	struct Rgba {
		Rgb color;
		double alpha;
	};

	using Color = std::variant<std::monostate, std::string, Rgb, Rgba>;
	const Color NoneColor{};

	void RenderColor(std::ostream& out, std::monostate);

	void RenderColor(std::ostream& out, const std::string& value);

	void RenderColor(std::ostream& out, Rgb rgb);

	void RenderColor(std::ostream& out, Rgba rgba);

	void RenderColor(std::ostream& out, const Color& color);



	class Object {
	public:
		virtual void Render(std::ostream& out) const = 0;
		virtual std::unique_ptr<Object> Copy() const = 0;
		virtual ~Object() = default;
	};

	template <typename Owner>
	class CopyableObject : public Object {
	public:
		std::unique_ptr<Object> Copy() const override {
			return std::make_unique<Owner>(static_cast<const Owner&>(*this));
		}
		virtual ~CopyableObject() = default;
	};

	template <typename Owner>
	class PathProps {
	public:
		Owner& SetFillColor(const Color& color) {
			fill_color_ = color;
			return AsOwner();
		}
		Owner& SetStrokeColor(const Color& color) {
			stroke_color_ = color;
			return AsOwner();
		}
		Owner& SetStrokeWidth(double value) {
			stroke_width_ = value;
			return AsOwner();
		}
		Owner& SetStrokeLineCap(const std::string& value) {
			stroke_line_cap_ = value;
			return AsOwner();
		}
		Owner& SetStrokeLineJoin(const std::string& value) {
			stroke_line_join_ = value;
			return AsOwner();
		}
		void RenderAttrs(std::ostream& out) const {
			out << "fill=\"";
			RenderColor(out, fill_color_);
			out << "\" ";
			out << "stroke=\"";
			RenderColor(out, stroke_color_);
			out << "\" ";
			out << "stroke-width=\"" << stroke_width_ << "\" ";
			if (stroke_line_cap_) {
				out << "stroke-linecap=\"" << *stroke_line_cap_ << "\" ";
			}
			if (stroke_line_join_) {
				out << "stroke-linejoin=\"" << *stroke_line_join_ << "\" ";
			}
		}

	private:
		Color fill_color_;
		Color stroke_color_;
		double stroke_width_ = 1.0;
		std::optional<std::string> stroke_line_cap_;
		std::optional<std::string> stroke_line_join_;

		Owner& AsOwner() {
			return static_cast<Owner&>(*this);
		}
	};

	class Circle : public CopyableObject<Circle>, public PathProps<Circle> {
	public:
		Circle& SetCenter(Point point);
		Circle& SetRadius(double radius);
		void Render(std::ostream& out) const override;

	private:
		Point center_;
		double radius_ = 1;
	};

	class Polyline : public CopyableObject<Polyline>, public PathProps<Polyline> {
	public:
		Polyline& AddPoint(Point point);
		void Render(std::ostream& out) const override;

	private:
		std::vector<Point> points_;
	};

	class Rectangle : public CopyableObject<Rectangle>, public PathProps<Rectangle> {
	public:
		Rectangle& SetCorner(Point point);
		Rectangle& SetWidth(double size);
		Rectangle& SetHeight(double size);
		void Render(std::ostream& out) const override;
	private:
		Point lu_corner;
		double w = 0;
		double h = 0;
	};

	class Text : public CopyableObject<Text>, public PathProps<Text> {
	public:
		Text& SetPoint(Point point);
		Text& SetOffset(Point point);
		Text& SetFontSize(uint32_t size);
		Text& SetFontFamily(const std::string& value);
		Text& SetFontWeight(const std::string& value);
		Text& SetData(const std::string& data);
		void Render(std::ostream& out) const override;

	private:
		Point point_;
		Point offset_;
		uint32_t font_size_ = 1;
		std::optional<std::string> font_family_;
		std::optional<std::string> font_weight_;
		std::string data_;
	};

	class Document : public CopyableObject<Document> {
	public:
		Document() = default;
		Document(const Document& other);
		Document& operator=(const Document& other);

		Document(Document&&) = default;
		Document& operator=(Document&&) = default;

		template <typename ObjectType>
		void Add(ObjectType object) {
			objects_.push_back(std::make_unique<ObjectType>(std::move(object)));
		}

		void Render(std::ostream& out) const override;

	private:
		std::vector<std::unique_ptr<Object>> objects_;
	};
}