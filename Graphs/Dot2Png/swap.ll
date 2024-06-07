; ModuleID = 'swap.ll'
source_filename = "swap.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @swap(i8** noundef %p, i8** noundef %q) #0 {
entry:
  %0 = load i8*, i8** %p, align 8
  %1 = load i8*, i8** %q, align 8
  store i8* %1, i8** %p, align 8
  store i8* %0, i8** %q, align 8
  ret void
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 {
entry:
  %aa = alloca i8, align 1
  %bb = alloca i8, align 1
  %pa = alloca i8*, align 8
  %pb = alloca i8*, align 8
  store i8 49, i8* %aa, align 1
  store i8 50, i8* %bb, align 1
  store i8* %aa, i8** %pa, align 8
  store i8* %bb, i8** %pb, align 8
  call void @swap(i8** noundef %pa, i8** noundef %pb)
  ret i32 0
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 14.0.0"}
