; ModuleID = 'a.c'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

@.str = private unnamed_addr constant [15 x i8] c"hello pass !!!\00", align 1
@.str1 = private unnamed_addr constant [6 x i8] c"j:%d\0A\00", align 1

define i32 @a(i32 %t) nounwind {
entry:
  %t.addr = alloca i32, align 4
  store i32 %t, i32* %t.addr, align 4
  %0 = load i32* %t.addr, align 4
  ret i32 %0
}

define i32 @main() nounwind {
entry:
  %retval = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %b = alloca i32 (i32)*, align 4
  store i32 0, i32* %retval
  %call = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([15 x i8]* @.str, i32 0, i32 0))
  %call1 = call i32 @a(i32 10)
  store i32 0, i32* %i, align 4
  store i32 0, i32* %j, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32* %i, align 4
  %cmp = icmp slt i32 %0, 10
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %1 = load i32* %j, align 4
  %inc = add nsw i32 %1, 1
  store i32 %inc, i32* %j, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %2 = load i32* %i, align 4
  %inc2 = add nsw i32 %2, 1
  store i32 %inc2, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %3 = load i32* %j, align 4
  %call3 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([6 x i8]* @.str1, i32 0, i32 0), i32 %3)
  ret i32 0
}

declare i32 @printf(i8*, ...)
